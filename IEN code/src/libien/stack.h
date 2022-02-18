/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
Intelligent_Eco_Networking_IEN_an_Advanced_Future_Internet_of_intelligence_for_Digital_Social_Economic_Ecosystem
**************************************************/
#pragma once
#include <log4cplus/logger.h>
#include <vector>
#include "./common/common.h"
#include "./base/base.h"
#include "./protocol/protocol.h"
#include "./interface/net_manager.h"
#include "./sn_client/client.h"
#include "./tunnel/tunnel_manager.h"
#include "./history/keymanager.h"

namespace ien {
	struct TlsEncodeBuffer {
		uint8_t udpEncodeBuffer[net::UdpInterface::mtu];
		uint8_t udpDecryptBuffer[net::UdpInterface::maxDataSize];
		uint8_t tcpEncodeBuffer[net::TcpInterface::maxDataSize];
	};

	class Stack {
	public:
		log4cplus::Logger& logger();

		peer::Store& peerStore() {
			return this->_peerStore;
		}

		net::NetManager* netManager() const {
			return this->_netManager;
		}

		tunnel::TunnelManager* tunnelManager() const {
			return this->_tunnelManager;
		}

		history::KeyManager* keyManager() const {
			return this->_keyManager;
		}

		sn::SnClient* snClient() const {
			return this->_snClient;
		}

		const peer::Const& localConst() const;
		const peer::Secret& secret() const;

		TlsEncodeBuffer* tlsEncodeBuffer();

	public:
		void onCalled(protocol::SnCalled* called);
		void onOnline(std::shared_ptr<peer::Info> sn);
		void onOffline(std::shared_ptr<peer::Info> sn);

	private:
		history::KeyManager* _keyManager;
		peer::Store _peerStore;
		net::NetManager* _netManager;
		tunnel::TunnelManager* _tunnelManager;
		sn::SnClient* _snClient;
	};
}
