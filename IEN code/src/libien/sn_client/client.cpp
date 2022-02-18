#include "../stack.h"
#include "./client.h"

namespace ien{
	namespace sn {

		SnClient::SnClient(Stack* stack)
		: _stack(stack) 
		, _status(IEN_SUPER_NODE_CLIENT_STATUS_STOPPED)
		, _pingMgr(new PingManager(stack)) {
			uv_rwlock_init(&this->_seqLock);
		}

		SnClient::~SnClient() {
			uv_rwlock_destroy(&this->_seqLock);
		}

		uint32_t SnClient::start() {
			//TODO:在多线程环境下，状态判断未加保护是有潜在隐患的，应该用atomic 比较
			if (this->_status == IEN_SUPER_NODE_CLIENT_STATUS_RUNNING) {
				LOG4CPLUS_INFO(this->_stack->logger(), LOG4CPLUS_TEXT("sn client:") << this << LOG4CPLUS_TEXT(", already started."));
				return ICN_RESULT_INVALID_STATE;
			}

			this->_timer = Timer::setInterval(100, std::static_pointer_cast<TimerCallback>(this->_pingMgr));
			this->_status = IEN_SUPER_NODE_CLIENT_STATUS_RUNNING;
			return ICN_RESULT_SUCCESS;
		}

		uint32_t SnClient::stop() {
			if (this->_status == IEN_SUPER_NODE_CLIENT_STATUS_STOPPED) {
				LOG4CPLUS_WARN(this->_stack->logger(), LOG4CPLUS_TEXT("client:") << this << LOG4CPLUS_TEXT(", already stopped."));
				return ICN_RESULT_INVALID_STATE;
			}

			this->_status = IEN_SUPER_NODE_CLIENT_STATUS_STOPPED;
			IcnClearTimer(this->_timer);
			this->_timer = NULL;
			return ICN_RESULT_SUCCESS;
		}


		uint32_t SnClient::pushUdpPackage(
			const protocol::Package* package,
			const endpoint::Endpoint& fromEndpoint,
			std::shared_ptr<net::UdpInterface>& udpInterface,
			const uint8_t* aesKey,
			bool* outHandled) {
			{
			#ifndef BLOG_DISABLE
				char fromEndpointStr[endpoint::maxChars + 1];
				LOG4CPLUS_INFO(this->_stack->logger(), 
					LOG4CPLUS_TEXT("client:") << this << 
					LOG4CPLUS_TEXT("cmdtype:") << package->cmdtype << 
					LOG4CPLUS_TEXT("from:") << (fromEndpoint.toString(fromEndpointStr), fromEndpointStr)
				);
			#endif
			}

			switch (package->cmdtype) {
			case protocol::SnPingResp::command: {
				this->_pingMgr->pingResponsed((protocol::SnPingResp*)package, fromEndpoint, udpInterface, outHandled);
				break;
			}
			
			case protocol::SnCallResp::command: {
				protocol::SnCallResp* callResp = (protocol::SnCallResp*)package;
				std::shared_ptr<CallSession> callSession = NULL;
				uv_rwlock_rdlock(&this->_seqLock);
				{
					std::map<uint32_t, std::shared_ptr<CallSession>>::iterator iter = this->_seqMap.find(callResp->seq);
					if (iter != this->_seqMap.end()) {
						callSession = iter->second;
					}
				}
				uv_rwlock_rdunlock(&this->_seqLock);
				if (callSession.get() != NULL) {
					callSession->onSnCallResp(callResp, fromEndpoint, udpInterface, outHandled);
					*outHandled = true;
				}
				break;
			}
			case protocol::SnCalled::command: {
				protocol::SnCalled* calledReq = (protocol::SnCalled*)package;
				this->_sendCalledResp(
					calledReq,
					&fromEndpoint,
					udpInterface,
					aesKey);
				this->_stack->onCalled(calledReq);
				*outHandled = TRUE;
				break;
			}
			default:
				assert(FALSE);
			}
			return ICN_RESULT_SUCCESS;
		}


		void SnClient::_sendCalledResp(
			protocol::SnCalled* calledReq,
			const endpoint::Endpoint* snEndpoint,
			std::shared_ptr<net::UdpInterface>& udpInterface,
			const uint8_t* aesKey) {
		
			protocol::SnCalledResp calledRespPkg;
			calledRespPkg.seq = calledReq->seq;

			TlsEncodeBuffer* tls = this->_stack->tlsEncodeBuffer();
			size_t encodedSize = sizeof(tls->udpEncodeBuffer);
			net::UdpInterface::box(
				(const protocol::Package**)(&calledRespPkg),
				1,
				NULL,
				aesKey,
				tls->udpEncodeBuffer,
				&encodedSize);

			udpInterface->sendTo(udpInterface, *snEndpoint, tls->udpEncodeBuffer, encodedSize);
		}


		std::shared_ptr<CallSession> SnClient::call(
			std::shared_ptr<peer::Info> localPeerInfo,
			peer::Id& remotePeerid,
			std::vector<endpoint::Endpoint>& reverseEndpointArray,
			std::shared_ptr<peer::Info> snPeerInfo,
			bool alwaysCall,
			std::shared_ptr<CallSessionCallback> callback) {
			uint32_t nextSeq = this->seq().next();

			std::shared_ptr<CallSession> callSession = std::make_shared<CallSession>(
				this->_stack,
				localPeerInfo,
				remotePeerid,
				reverseEndpointArray,
				snPeerInfo,
				nextSeq, 
				alwaysCall,
				callback);
			

			uv_rwlock_wrlock(&this->_seqLock);
			{
				this->_seqMap.insert(std::make_pair(nextSeq, callSession));
			}
			uv_rwlock_wrunlock(&this->_seqLock);

			return callSession;
		}

	}
}