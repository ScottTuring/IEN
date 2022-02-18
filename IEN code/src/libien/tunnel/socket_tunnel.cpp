#include "./socket_tunnel.h"

namespace ien {
	namespace tunnel {
		SocketTunnel::~SocketTunnel() {

		}

		const endpoint::Endpoint& SocketTunnel::local() const {
			return this->_local;
		}


		const endpoint::Endpoint& SocketTunnel::remote() const {
			return this->_remote;
		}

		const peer::Id& SocketTunnel::remotePeerid() const {
			return this->_remotePeerid;
		}

		bool SocketTunnel::isUdp() const {
			return this->_flags & flagUdp;
		}

		bool SocketTunnel::isTcp() const {
			return this->_flags & flagTcp;
		}
	}
}