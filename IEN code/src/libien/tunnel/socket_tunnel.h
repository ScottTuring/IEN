/*************************************************
Copyright��������ѧ�����о���Ժ PKUSZ
Author�����������������������������ص�ʵ���ң�ICNlab��
			   IENС��
web��http://www.ienlab.com/
Description:������̬���磨Intelligent Eco Networking��
**************************************************/
#pragma once
#include <memory>
#include "../common/common.h"
#include "../base/base.h"



namespace ien {
	namespace tunnel {
		class SocketTunnel {
		public:
			virtual ~SocketTunnel();
			const endpoint::Endpoint& local() const;
			const endpoint::Endpoint& remote() const;
			const peer::Id& remotePeerid() const;
			bool isUdp() const;
			bool isTcp() const;

		protected:
			static const uint32_t flagUdp = (1 << 0);
			static const uint32_t flagTcp = (1 << 1);
			uint32_t _flags;
		private:
			peer::Id _remotePeerid;
			endpoint::Endpoint _local;
			endpoint::Endpoint _remote;
			int64_t _lastRecv;
		};
	}
}