/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <uv.h>
#include <memory>
#include <vector>
#include "../common/common.h"
#include "../base/base.h"
#include "../thread/timer.h"
#include "../protocol/protocol.h"
#include "../interface/net_manager.h"

namespace ien {
	namespace sn {

		class CallSessionCallback {
		public:
			virtual void onFinish(
				uint32_t errorCode,
				std::shared_ptr<peer::Info> remotePeerInfo
			) = 0;
		};

		class CallSession
			: public TimerCallback {
		public:
			CallSession(
				Stack* stack, 
				std::shared_ptr<peer::Info> localPeerInfo,
				peer::Id& remotePeerid,
				std::vector<endpoint::Endpoint>& reverseEndpointArray,
				std::shared_ptr<peer::Info> snPeerInfo,
				uint32_t startSeq, 
				bool alwaysCall,
				std::shared_ptr<CallSessionCallback> callback
			);

			std::shared_ptr<protocol::Package> callPkg();
			uint32_t start(std::shared_ptr<CallSession> ref, std::shared_ptr<Buffer> payload);
			void onTimer();
			uint32_t onSnCallResp(
				protocol::SnCallResp* package,
				const endpoint::Endpoint& fromEndpoint,
				std::shared_ptr<net::UdpInterface> fromInterface,
				bool* outHandled);
		private:
			void _stop();
			void _resendUdp();
		private:
			Stack* _stack;
			uint32_t _seq;
			std::shared_ptr<peer::Info> _localPeerInfo;
			peer::Id _remotePeerid;
			bool _alwaysCall;

			int64_t _endTime;
			ICN_TIMER_HANDLE _timer;

			std::shared_ptr<CallSessionCallback> _callback;
			std::shared_ptr<protocol::Package> _pkgs[2];

			std::shared_ptr<peer::Info> _snPeerInfo;
			volatile int64_t _lastCallTime;

			uint32_t _resendInterval;
			uint8_t _aesKey[crypto::aesBytes];

			volatile int64_t _lastRespTime;
		};
	}
}