#include "../stack.h"
#include "./ping.h"

namespace ien {
	namespace sn {
		PingClient::PingClient(Stack* stack, std::shared_ptr<peer::Info> snPeerInfo)
			: _stack(stack)
			, _snPeerInfo(snPeerInfo) {
			
			const std::vector<std::shared_ptr<net::UdpInterface>>& interfaceList = _stack->netManager()->udpInterfaces();
			for (size_t i = 0; i < interfaceList.size(); i++) {
				PingSession session;
				session.outEndpoint = interfaceList[i]->local();
				session.handle = interfaceList[i];
				session.lastPingTime = session.lastRespTime = 0;
				this->_sessions.push_back(session);
			}

			bool isNew = true;
			bool isConfirmed = false;
			stack->keyManager()->getKeyByRemote(snPeerInfo->id(), this->_aesKey, &isNew, &isConfirmed, true);
		}

		void PingClient::start() {
			uint32_t oldStatus = IcnAtomicCompareAndSwap32(
				(int32_t*)&this->_status,
				PING_CLIENT_STATUS_INIT,
				PING_CLIENT_STATUS_CONNECTING);

			if (oldStatus == PING_CLIENT_STATUS_INIT) {
				this->updateStatus();
			}
		}

		void PingClient::updateStatus() {
			if (this->_status == PING_CLIENT_STATUS_OFFLINE) {
				return;
			}

			int64_t now = IcnTimeGetNow(false) / 1000;
			int pingInterval = 600000;

			switch (this->_status) {
			case PING_CLIENT_STATUS_INIT: // fall through
			case PING_CLIENT_STATUS_CONNECTING: {
				if (this->_lastPingTime > 0 &&
					now - this->_lastPingTime > 300000) {
					int32_t oldStatus = IcnAtomicExchange32((int32_t*)&this->_status, PING_CLIENT_STATUS_OFFLINE);
					if (oldStatus != PING_CLIENT_STATUS_OFFLINE) {
						
						this->_stack->onOffline(this->_snPeerInfo);
					}
				}
				else {
					pingInterval = 500;
				}
			}
			break;
			case PING_CLIENT_STATUS_ONLINE: {
				if (now - this->_lastRespTime > 300000) {
					int32_t oldStatus = IcnAtomicExchange32((int32_t*)&this->_status, PING_CLIENT_STATUS_OFFLINE);
					if (oldStatus != PING_CLIENT_STATUS_OFFLINE) {
						this->_stack->onOffline(this->_snPeerInfo);
					}
				}
			}
			break;
			case PING_CLIENT_STATUS_OFFLINE: // fall through
				break;
			default:
				return;
			}

			if (this->_status == PING_CLIENT_STATUS_OFFLINE) {
				return;
			}

			if (now - this->_lastPingTime > pingInterval) {
				this->sendPing();
			}
		}

		void PingClient::stop() {
			if (this->_status == PING_CLIENT_STATUS_OFFLINE) {
				return;
			}

			int32_t oldStatus = IcnAtomicExchange32((int32_t*)&this->_status, PING_CLIENT_STATUS_OFFLINE);
			if (oldStatus != PING_CLIENT_STATUS_OFFLINE) {
				this->_stack->onOffline(this->_snPeerInfo);
			}
		}

		bool PingClient::onResponse(
			protocol::SnPingResp* package,
			const endpoint::Endpoint* fromEndpoint,
			std::shared_ptr<net::UdpInterface> fromInterface,
			endpoint::Endpoint* newOutEndpoint) {

		#ifndef BLOG_DISABLE
			char fromEpStr[endpoint::maxChars + 1];
			LOG4CPLUS_INFO(this->_stack->logger(),
				"sn:" << this << "responce from " << (fromEndpoint->toString(fromEpStr), fromEpStr));
		#endif

			int64_t now = IcnTimeGetNow(false) / 1000;
			this->_lastRespTime = now;

			bool isEndpointChange = false;
			
			// 找到interface对应的session更新其中信息
			const endpoint::Endpoint& outEndpoint = package->endpointArray.at(0);
			for (size_t i = 0; i < this->_sessions.size(); i++) {
				PingSession& session = this->_sessions.at(i);
				if (session.handle == fromInterface) {
					isEndpointChange = outEndpoint.compare(&session.outEndpoint, false) != 0;

					if (isEndpointChange) {
						session.outEndpoint = outEndpoint;
						session.outEndpoint.flag &= ~endpoint::flagSigned;
						session.outEndpoint.flag |= endpoint::flagStaticWan;
						*newOutEndpoint = session.outEndpoint;
						// 前半段网络地址，后半段主机地址
						if (outEndpoint.flag & endpoint::ipVersion6) {
							memcpy(session.outEndpoint.addressV6 + 8,
								fromInterface->local().addressV6 + 8,
								8);
						}

					#ifndef BLOG_DISABLE
						char foundEpStr[endpoint::maxChars + 1];
						LOG4CPLUS_INFO(this->_stack->logger(),
							"sn:" << this << "found new endpoint " << (session.outEndpoint.toString(foundEpStr), foundEpStr) << 
							" from " << fromEpStr);
					#endif
					}
					if (session.lastRespTime == 0) {
						session.remoteEndpoint = *fromEndpoint;
					}
					session.lastRespTime = now;
				}
			}

			int32_t oldStatus = IcnAtomicCompareAndSwap32(
				(int32_t*)&this->_status,
				PING_CLIENT_STATUS_CONNECTING,
				PING_CLIENT_STATUS_ONLINE);

			if (oldStatus == PING_CLIENT_STATUS_CONNECTING) {
				this->_stack->onOnline(this->_snPeerInfo);
			}

			return isEndpointChange;
		}
		
		void PingClient::sendPing() {
			uint32_t result = ICN_RESULT_SUCCESS;
			std::shared_ptr<peer::Info> localPeer = this->_stack->peerStore().local();

			// fromPeerid被peerinfo字段包含，不专门填充
			protocol::Package* pkgs[2] = { NULL, NULL };
			int pkgCount = 0;
			uint32_t seq = this->_stack->snClient()->seq().next();
			protocol::ExchangeKey exchgKey;

			exchgKey.peerInfo = localPeer;
			exchgKey.seq = seq;
			exchgKey.sendTime = IcnTimeGetNow(false);
			pkgs[0] = (protocol::Package*)&exchgKey;
			pkgCount++;

			protocol::SnPing pingPkg;
			pingPkg.seq = seq;
			pingPkg.peerInfo = localPeer;
			pkgs[pkgCount] = (protocol::Package*)&pingPkg;
			pkgCount++;

			TlsEncodeBuffer* tls = this->_stack->tlsEncodeBuffer();
			size_t encodedSize = sizeof(tls->udpEncodeBuffer);
			
			
			result = net::UdpInterface::box(
				this->_stack, 
				(const protocol::Package**)pkgs,
				pkgCount,
				NULL,
				this->_aesKey,
				&this->_snPeerInfo->constInfo(), 
				tls->udpEncodeBuffer, 
				&encodedSize);
			

			if (result != ICN_RESULT_SUCCESS) {
				return;
			}

			int64_t now = IcnTimeGetNow(false) / 1000;
			this->_lastPingTime = now;

			for (size_t i = 0; i < this->_sessions.size(); i++) {
				PingSession& session = this->_sessions.at(i);
				std::shared_ptr<net::UdpInterface> udpInterface = session.handle;

				const endpoint::Endpoint* toEndpointArray = &session.remoteEndpoint;
				size_t toEndpointCount = 1;
				if (session.lastRespTime == 0) {
					toEndpointArray = this->_snPeerInfo->endpointList().data();
					toEndpointCount = this->_snPeerInfo->endpointList().size();
				}
				session.lastPingTime = now;
				for (size_t j = 0; j < toEndpointCount; j++) {
					const endpoint::Endpoint* toEp = toEndpointArray + j;
					if ((toEp->flag & endpoint::protocolUdp) == 0 ||
						!toEp->isSameIpVersion(&udpInterface->local())) {
						continue;
					}
					udpInterface->sendTo(udpInterface, *toEp, tls->udpEncodeBuffer, encodedSize);
				}
			}
		}

		PingManager::PingManager(Stack* stack)
			: _stack(stack) {
			uv_mutex_init(&this->_clientsLock);
		}

		PingManager::~PingManager() {
			uv_mutex_destroy(&this->_clientsLock);
			for (int i = 0; i < this->_maxPingClientCount; i++) {
				std::shared_ptr<PingClient> pingClient = this->_pingClients[i];
				if (pingClient.get() != NULL) {
					this->_pingClients[i] = NULL;
					pingClient->stop();
				}
			}
		}

		uint32_t PingManager::pingResponsed(
			protocol::SnPingResp* package,
			const endpoint::Endpoint& fromEndpoint,
			std::shared_ptr<net::UdpInterface> fromInterface,
			bool* outHandled) {

			std::shared_ptr<PingClient> targetPingClient = NULL;

			std::shared_ptr<PingClient> pingClients[16] = { NULL };
			int snCount = 0;

			uv_mutex_lock(&this->_clientsLock);
			snCount = this->_maxPingClientCount;
			for (int i = 0; i < this->_maxPingClientCount; i++) {
				pingClients[i] = this->_pingClients[i];
			}
			uv_mutex_unlock(&this->_clientsLock);

			for (int i = 0; i < snCount; i++) {
				if (pingClients[i] &&
					pingClients[i]->snPeerInfo()->id().compare(&package->peerInfo->id()) == 0 &&
					(pingClients[i]->status() == PingClient::Status::PING_CLIENT_STATUS_ONLINE || pingClients[i]->status() == PingClient::Status::PING_CLIENT_STATUS_CONNECTING)) {
					targetPingClient = pingClients[i];
					break;
				}
			}

			if (targetPingClient.get() != NULL) {
				endpoint::Endpoint newOutEndpoint;
				bool isNewEndpoint = targetPingClient->onResponse(
					package, 
					&fromEndpoint, 
					fromInterface, 
					&newOutEndpoint);
				if (isNewEndpoint) {
					// 更新localPeerInfo，并签名
					std::shared_ptr<peer::Info> localPeer = this->_stack->peerStore().local();

					if (!localPeer->hasEndpoint(newOutEndpoint)) {
						peer::InfoBuilder peerInfoBuilder = peer::InfoBuilder::begin(peer::InfoBuilder::flagHasSnList | peer::InfoBuilder::flagHasSnInfoList);
						peerInfoBuilder.setConstInfo(this->_stack->localConst());

						for (int i = 0; i < snCount; i++) {
							if (pingClients[i]) {
								for (size_t si = 0; si < pingClients[i]->sessions().size(); si++) {
									PingSession& session = pingClients[i]->sessions().at(si);
									if (session.lastRespTime > 0) {
										peerInfoBuilder.addEndpoint(session.outEndpoint);
									}
								}
							}
						}
						std::shared_ptr<peer::Info> newLocalPeer = peerInfoBuilder.finish(&this->_stack->secret());

						if (newLocalPeer.get() != NULL) {
							this->_stack->peerStore().updateLocal(newLocalPeer);
						}
					}

					for (int i = 0; i < snCount; i++) {
						if (pingClients[i] &&
							(pingClients[i]->status() == PingClient::Status::PING_CLIENT_STATUS_ONLINE || pingClients[i]->status() == PingClient::Status::PING_CLIENT_STATUS_CONNECTING)) {
							pingClients[i]->sendPing();
						}
					}
				}
			}

			return ICN_RESULT_SUCCESS;
		}

		void PingManager::onTimer() {
			// 1.检查所有在线SN是否掉线
			// 2.搜索更新SN列表
			std::shared_ptr<PingClient> offlineClients[16];
			int offlineCount = 0;
			std::shared_ptr<PingClient> activeClients[16];
			int activeCount = 0;

			uv_mutex_lock(&this->_clientsLock);

			for (int i = 0; i < this->_maxPingClientCount; i++) {
				std::shared_ptr<PingClient> pingClient = this->_pingClients[i];
				int32_t status = PingClient::Status::PING_CLIENT_STATUS_INIT;
				if (pingClient != NULL) {
					status = pingClient->status();
					if (status == PingClient::Status::PING_CLIENT_STATUS_OFFLINE) {
						this->_pingClients[i] = NULL;
					}
				}

				if (status == PingClient::Status::PING_CLIENT_STATUS_OFFLINE) {
					offlineClients[offlineCount++] = pingClient;
					if (offlineCount >= ICN_ARRAYSIZE(offlineClients)) {
						break;
					}
				}
				else if (status != PingClient::Status::PING_CLIENT_STATUS_INIT) {
					activeClients[activeCount++] = pingClient;
					if (activeCount >= ICN_ARRAYSIZE(activeClients)) {
						break;
					}
				}
			}

			uv_mutex_unlock(&this->_clientsLock);

			for (int i = 0; i < activeCount; i++) {
				activeClients[i]->updateStatus();
			}
		}
	}
}