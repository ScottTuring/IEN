/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <memory>
#include "../common/common.h"
#include "../base/base.h"
#include "../protocol/protocol.h"
#include "../interface/net_manager.h"

namespace ien {
	namespace tunnel {
		class TunnelContainer {
		public:
			const peer::Id& remote() const;
			const peer::Id& local() const;
			
			uint32_t pushUdpPackage(
				const protocol::Package* package,
				std::shared_ptr<net::UdpInterface>& udpInterface,
				const endpoint::Endpoint& fromEndpoint,
				bool* outHandled);
			uint32_t pushTcpFirstPackage(
				const protocol::Package* package,
				std::shared_ptr<net::TcpInterface>& tcpInterface
			);
			uint32_t pushSessionPackage(
				const protocol::Package* package,
				bool* outHandled);
		};
	}
}