#include "./net_manager.h"

namespace ien {
	namespace net {
		void NetManager::_updateBoundAddr(const endpoint::Endpoint& ep) {
			endpoint::Endpoint ba = ep;
			ba.flag &= ~(endpoint::protocolTcp | endpoint::protocolUdp);
			ba.port = 0;
			for (size_t ix = 0; ix < this->_boundAddr.size(); ++ix) {
				if (this->_boundAddr.at(ix).compare(&ba, true) == 0) {
					return;
				}
			}
			this->_boundAddr.push_back(ba);
		}

		uint32_t NetManager::_openUdp(const endpoint::Endpoint& local) {
			std::shared_ptr<UdpInterface> udpInterface = UdpInterface::open(this->_stack, local);
			if (!udpInterface.get()) {
				return ICN_RESULT_FAILED;
			}
			this->_udpList.push_back(udpInterface);
			this->_updateBoundAddr(local);
			return ICN_RESULT_SUCCESS;
		}

		uint32_t NetManager::_openTcp(const endpoint::Endpoint& local) {
			std::shared_ptr<TcpListener> tcpListener = TcpListener::open(this->_stack, local);
			if (!tcpListener.get()) {
				return ICN_RESULT_FAILED;
			}
			this->_tcpList.push_back(tcpListener);
			this->_updateBoundAddr(local);
			return ICN_RESULT_SUCCESS;
		}

		int32_t NetManager::start(
			Stack* stack, 
			const std::vector<endpoint::Endpoint>& endpoints, 
			std::vector<uint32_t>& results) {
			for (size_t ix = 0; ix < endpoints.size(); ++ix) {
				const endpoint::Endpoint& ep = endpoints.at(ix);
				if (ep.flag & endpoint::protocolUdp) {
					results.push_back(this->_openUdp(ep));
				}
				else {
					results.push_back(this->_openTcp(ep));
				}
			}
			return ICN_RESULT_SUCCESS;
		}

		const std::vector<endpoint::Endpoint>& NetManager::boundAddr() const {
			return this->_boundAddr;
		}

		const std::vector<std::shared_ptr<UdpInterface>>& NetManager::udpInterfaces() const {
			return this->_udpList;
		}

		const std::shared_ptr<UdpInterface> NetManager::udpInterface(const endpoint::Endpoint& local) const {
			for (size_t ix = 0; ix < this->_udpList.size(); ++ix) {
				std::shared_ptr<UdpInterface> ui = this->_udpList[ix];
				if (ui->local().compare(&local, false) == 0) {
					return ui;
				}
			}
			return NULL;
		}

		const std::vector<std::shared_ptr<TcpListener>>& NetManager::tcpListeners() const {
			return this->_tcpList;
		}

		std::shared_ptr<TcpInterface> NetManager::tcp(
			const endpoint::Endpoint& local, 
			const endpoint::Endpoint& remote) {
			return TcpInterface::create(this->_stack, local, remote);
		}
	}
}