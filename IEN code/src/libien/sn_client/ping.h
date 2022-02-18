/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <vector>
#include <uv.h>
#include "../common/common.h"
#include "../base/base.h"
#include "../interface/net_manager.h"
#include "../thread/timer.h"
#include "../protocol/protocol.h"
namespace ien {
	namespace sn {

		struct PingSession {
			std::shared_ptr<net::UdpInterface> handle;
			endpoint::Endpoint outEndpoint; // 出口地址
			endpoint::Endpoint remoteEndpoint;
			int64_t lastPingTime;
			int64_t lastRespTime;
		};

		
		class PingClient {
		public:
			enum Status {
				PING_CLIENT_STATUS_INIT = 0,
				PING_CLIENT_STATUS_CONNECTING = 1,
				PING_CLIENT_STATUS_ONLINE = 2,
				PING_CLIENT_STATUS_OFFLINE = 3,
			};
			PingClient(Stack* stack, std::shared_ptr<peer::Info> snPeerInfo);
			int32_t status() {
				return this->_status;
			}
			std::shared_ptr<peer::Info> snPeerInfo() {
				return this->_snPeerInfo;
			}
			void updateStatus();
			std::vector<PingSession>& sessions() {
				return this->_sessions;
			}

			void start();
			
			void stop();
			bool onResponse(
				protocol::SnPingResp* package,
				const endpoint::Endpoint* fromEndpoint,
				std::shared_ptr<net::UdpInterface> fromInterface,
				endpoint::Endpoint* newOutEndpoint);
			void sendPing();
		private:

			Stack* _stack;
			std::shared_ptr<peer::Info> _snPeerInfo;
			int64_t _lastPingTime;
			int64_t _lastRespTime;

			std::vector<PingSession> _sessions;
			volatile int32_t _status;

			uint8_t _aesKey[crypto::aesBytes];
		};

		class PingManager: public TimerCallback {
		public:
			PingManager(Stack* stack);
			~PingManager();
			void onTimer();
			uint32_t pingResponsed(
				protocol::SnPingResp* package,
				const endpoint::Endpoint& fromEndpoint,
				std::shared_ptr<net::UdpInterface> fromInterface,
				bool* outHandled);
		private:
			Stack* _stack;
			uv_mutex_t _clientsLock;
			int _maxPingClientCount;
			std::shared_ptr<PingClient> _pingClients[16];
		};
	}
}