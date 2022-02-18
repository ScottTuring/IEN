/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
Intelligent Eco Networking IEN)_ Knowledge-Driven and Value-Oriented Future Internet Infrastructure
**************************************************/
#pragma once
#include <memory>
#include <vector>
#include <map>
#include <uv.h>
#include "../common/common.h"
#include "../base/base.h"
#include "../thread/timer.h"
#include "../protocol/protocol.h"
#include "../interface/net_manager.h"
#include "./call_session.h"
#include "./ping.h"

namespace ien {
	namespace sn {
		class SnClient {
			enum Status {
				IEN_SUPER_NODE_CLIENT_STATUS_STOPPED = 0,
				IEN_SUPER_NODE_CLIENT_STATUS_RUNNING = 1,
			};
		public:
			SnClient(Stack* stack);
			~SnClient();

			uint32_t start();
			uint32_t stop();
			
			SequenceCreator& seq() {
				return this->_seqCreator;
			}

			uint32_t updateSnList(const std::vector<std::shared_ptr<peer::Info>> snList);

			uint32_t pushUdpPackage(
				const protocol::Package* package, 
				const endpoint::Endpoint& fromEndpoint, 
				std::shared_ptr<net::UdpInterface>& udpInterface, 
				const uint8_t* aesKey, 
				bool* outHandled);
			
			std::shared_ptr<CallSession> call(
				std::shared_ptr<peer::Info> localPeerInfo,
				peer::Id& remotePeerid,
				std::vector<endpoint::Endpoint>& reverseEndpointArray,
				std::shared_ptr<peer::Info> snPeerInfo,
				bool alwaysCall,
				std::shared_ptr<CallSessionCallback> callback);

		private:
			void _sendCalledResp(
				protocol::SnCalled* calledReq,
				const endpoint::Endpoint* snEndpoint,
				std::shared_ptr<net::UdpInterface>& udpInterface,
				const uint8_t* aesKey);
		private:
			Stack* _stack;
			Status _status;
			std::shared_ptr<PingManager> _pingMgr;
			SequenceCreator _seqCreator;
			uv_rwlock_t _seqLock;
			std::map<uint32_t, std::shared_ptr<CallSession>> _seqMap;
			ICN_TIMER_HANDLE _timer;
		};
	}
}
