#include "../stack.h"
#include "./call_session.h"

namespace ien {
	namespace sn {
		CallSession::CallSession(
			Stack* stack,
			std::shared_ptr<peer::Info> localPeerInfo,
			peer::Id& remotePeerid,
			std::vector<endpoint::Endpoint>& reverseEndpointArray,
			std::shared_ptr<peer::Info> snPeerInfo,
			uint32_t startSeq, 
			bool alwaysCall, 
			std::shared_ptr<CallSessionCallback> callback)
			: _stack(stack)
			, _remotePeerid(remotePeerid)
			, _seq(startSeq)
			, _alwaysCall(alwaysCall)
			, _callback(callback)
			, _snPeerInfo(snPeerInfo) {

			// fromPeerid被peerinfo字段包含，忽略它
			std::shared_ptr<protocol::ExchangeKey> exchgKey = std::make_shared<protocol::ExchangeKey>();
			exchgKey->peerInfo = localPeerInfo;
			exchgKey->fromPeerid = localPeerInfo->id();
			exchgKey->seq = startSeq;
			exchgKey->sendTime = 0;

			std::shared_ptr<protocol::SnCall> callPkg = std::make_shared<protocol::SnCall>();
			if (alwaysCall) {
				callPkg->cmdflags |= protocol::SnCall::Flags::alwaysCall;
			}

			callPkg->seq = startSeq;
			callPkg->peerInfo = localPeerInfo;
			callPkg->fromPeerid = localPeerInfo->id();
			callPkg->toPeerid = remotePeerid;
			
			if (reverseEndpointArray.size() > 0) {
				callPkg->reverseEndpointArray = reverseEndpointArray;
				callPkg->cmdflags |= protocol::SnCall::Flags::reverseEndpointArray;
			}
			this->_pkgs[0] = std::static_pointer_cast<protocol::Package>(exchgKey);
			this->_pkgs[1] = std::static_pointer_cast<protocol::Package>(callPkg);
		}


		std::shared_ptr<protocol::Package> CallSession::callPkg() {
			return this->_pkgs[1];
		}


		void CallSession::_stop() {
			if (this->_timer) {
				IcnClearTimer(this->_timer);
				this->_timer = NULL;
			}
		}

		void CallSession::_resendUdp() {
			TlsEncodeBuffer* tls = this->_stack->tlsEncodeBuffer();
			
			const std::vector<std::shared_ptr<net::UdpInterface>>& udpList = this->_stack->netManager()->udpInterfaces();
			const std::vector<endpoint::Endpoint>& epList = this->_snPeerInfo->endpointList();

			size_t encodedSize = sizeof(tls->udpEncodeBuffer);

			protocol::Package* pkgs[2] = { this->_pkgs[0].get(), this->_pkgs[1].get() };

			protocol::ExchangeKey* exchgPkg = (protocol::ExchangeKey*)pkgs[0];
			exchgPkg->sendTime = IcnTimeGetNow(false);

			uint32_t result = net::UdpInterface::box(
				this->_stack, 
				(const protocol::Package**)pkgs,
				2,
				NULL,
				this->_aesKey,
				&this->_snPeerInfo->constInfo(), 
				tls->udpEncodeBuffer,
				&encodedSize);
			
			IcnAtomicExchange64(&this->_lastCallTime, IcnTimeGetNow(false));
			if (result != ICN_RESULT_SUCCESS) {
				LOG4CPLUS_WARN(this->_stack->logger(),
					LOG4CPLUS_TEXT("session:") << this <<
					LOG4CPLUS_TEXT(", seq:") << this->_seq <<
					LOG4CPLUS_TEXT("udp package box failed : result:") << result);
				return;
			}

			for (size_t i = 0; i < udpList.size(); i++) {
				std::shared_ptr<net::UdpInterface> udpInterface = udpList.at(i);
				for (size_t j = 0; j < epList.size(); j++) {
					udpInterface->sendTo(udpInterface, epList.at(j), tls->udpEncodeBuffer, encodedSize);
				}
			}
		}

		uint32_t CallSession::onSnCallResp(
			protocol::SnCallResp* package,
			const endpoint::Endpoint& fromEndpoint,
			std::shared_ptr<net::UdpInterface> fromInterface,
			bool* outHandled) {

			if (IcnAtomicExchange64(&this->_lastRespTime, IcnTimeGetNow(false)) == 0) {
				this->_stop();
				std::shared_ptr<CallSessionCallback> callback = this->_callback;
				this->_callback = NULL;
				if (callback.get()) {
					uint32_t errorCode = ICN_RESULT_UNKNOWN;
					if (package->result == 0) {
						errorCode = ICN_RESULT_SUCCESS;
					}
					else if (package->result == 12) {
						errorCode = ICN_RESULT_NOT_FOUND;
					}
					else if (package->result == 2) {
						errorCode = ICN_RESULT_TOO_LARGE;
					}
					//TODO：这里传的SNCallResp里的PeerInfo，其EndPointList部分可能没有签名（是SN看到的）
					callback->onFinish(errorCode, package->toPeerInfo);
				}
			}

			return ICN_RESULT_SUCCESS;
		}

		void CallSession::onTimer() {
			int64_t now = IcnTimeGetNow(false);
			if (now > this->_endTime) {
				this->_stop();
				std::shared_ptr<CallSessionCallback> callback = this->_callback;
				this->_callback = NULL;
				callback->onFinish(ICN_RESULT_TIMEOUT, NULL);
			}
			else {
				if ((now - this->_lastCallTime) / 1000 > this->_resendInterval) {
					this->_resendUdp();
					this->_resendInterval *= 2;
				}
			}
		}

		uint32_t CallSession::start(std::shared_ptr<CallSession> ref, std::shared_ptr<Buffer> payload) {
			const int64_t now = IcnTimeGetNow(false);

			this->_endTime = now + 3000000;

			this->_resendInterval = 200000;

			bool isNewKey = true;
			bool isConfirmed = true;
			this->_stack->keyManager()->getKeyByRemote(
				this->_snPeerInfo->id(),
				this->_aesKey,
				&isNewKey,
				&isConfirmed,
				true);

			if (payload.get() != NULL) {
				std::static_pointer_cast<protocol::SnCall>(this->callPkg())->payload = payload;
			}

			// 没有优先启用地址就全地址广播
			this->_resendUdp();

			this->_timer = Timer::setInterval(100, std::static_pointer_cast<TimerCallback>(ref));

			return ICN_RESULT_SUCCESS;
		}
	}
}