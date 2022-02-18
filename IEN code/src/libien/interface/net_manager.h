/*************************************************
Copyright��������ѧ�����о���Ժ PKUSZ
Author�����������������������������ص�ʵ���ң�ICNlab��
			   IENС��
web��http://www.ienlab.com/
Description:������̬���磨Intelligent Eco Networking��
**************************************************/
#pragma once
#include <vector>
#include <memory>
#include "../common/common.h"
#include "../base/base.h"
#include "./udp_interface.h"
#include "./tcp_interface.h"

namespace ien {
	namespace net {
		class NetManager {
		public:
			int32_t start(Stack* stack, const std::vector<endpoint::Endpoint>& endpoints, std::vector<uint32_t>& results);
			const std::vector<endpoint::Endpoint>& boundAddr() const;
			const std::vector<std::shared_ptr<UdpInterface>>& udpInterfaces() const;
			const std::shared_ptr<UdpInterface> udpInterface(const endpoint::Endpoint& local) const;
			const std::vector<std::shared_ptr<TcpListener>>& tcpListeners() const;
			std::shared_ptr<TcpInterface> tcp(const endpoint::Endpoint& local, const endpoint::Endpoint& remote);

		private:
			void _updateBoundAddr(const endpoint::Endpoint& ep);
			uint32_t _openUdp(const endpoint::Endpoint& local);
			uint32_t _openTcp(const endpoint::Endpoint& local);

			Stack* _stack;
			std::vector<std::shared_ptr<UdpInterface>> _udpList;
			std::vector<std::shared_ptr<TcpListener>> _tcpList;
			std::vector<endpoint::Endpoint> _boundAddr;
		};
	}
}