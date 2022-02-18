#include "./tunnel_manager.h"
#include "../stack.h"

namespace ien {
	namespace tunnel {

		TunnelManager::TunnelManager(Stack* stack)
			: _stack(stack) {
			uv_rwlock_init(&this->_mapLock);
		}

		TunnelManager::~TunnelManager() {
			uv_rwlock_destroy(&this->_mapLock);
		}


		std::shared_ptr<TunnelContainer> TunnelManager::container(const peer::Id& remote) {
			std::shared_ptr<TunnelContainer> container(NULL);
			uv_rwlock_rdlock(&this->_mapLock);
			std::map<peer::Id, std::shared_ptr<TunnelContainer>>::iterator it = this->_containerEntry.find(remote);
			if (it != this->_containerEntry.end()) {
				container = std::shared_ptr<TunnelContainer>(it->second);
			} 
			uv_rwlock_rdunlock(&this->_mapLock);
			return container;
		}

		/*std::shared_ptr<TunnelContainer> TunnelManager::createContainer(const peer::Id& remote) {

		}

		void TunnelManager::removeContainer(const peer::Id& remote) {

		}*/
	}
}