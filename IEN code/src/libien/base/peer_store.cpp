#include "./peer_store.h"

namespace ien {
	namespace peer {
		Store::Store() {
			uv_rwlock_init(&this->_lock);
			uv_rwlock_init(&this->_localLock);
		}

		Store::~Store() {
			uv_rwlock_destroy(&this->_lock);
			uv_rwlock_destroy(&this->_localLock);
		}

		uint32_t Store::addStatic(std::shared_ptr<Info> peer) {
			uv_rwlock_rdlock(&this->_lock);
			this->_static.insert(std::make_pair(peer->id(), peer));
			uv_rwlock_rdunlock(&this->_lock);
			return ICN_RESULT_SUCCESS;
		}

		std::shared_ptr<Info> Store::getCachedOrStatic(const Id& peerid) {
			std::shared_ptr<Info> remoteInfo = this->getCached(peerid);
			if (!remoteInfo.get()) {
				return this->getStatic(peerid);
			}
			else {
				return remoteInfo;
			}
		}

		std::shared_ptr<Info> Store::getStatic(const Id& peerid) {
			std::shared_ptr<Info> peerInfo = NULL;
			uv_rwlock_rdlock(&this->_lock);
			std::map<Id, std::shared_ptr<Info>>::iterator iter = this->_static.find(peerid);
			if (iter != this->_static.end()) {
				peerInfo = iter->second;
			}
			uv_rwlock_rdunlock(&this->_lock);
			return peerInfo;
		}

		std::shared_ptr<Info> Store::getCached(const Id& peerid) {
			std::shared_ptr<Info> peerInfo = NULL;
			uv_rwlock_rdlock(&this->_lock);
			std::map<Id, std::shared_ptr<Info>>::iterator iter = this->_cached.find(peerid);
			if (iter != this->_cached.end()) {
				peerInfo = iter->second;
			}
			uv_rwlock_rdunlock(&this->_lock);
			return peerInfo;
		}

		// thread safe
		//TODO:这里是否应该用 LRU_Cache,现在看起来PeerInfoCache的大小可以是无限的
		uint32_t Store::addCached(std::shared_ptr<Info> peer, uint64_t expire) {
			bool updated = false;
			uv_rwlock_rdlock(&this->_lock);
			do {
				std::map<Id, std::shared_ptr<Info>>::iterator iter = this->_cached.find(peer->id());
				if (iter != this->_cached.end()) {
					if (iter->second->createTime() < peer->createTime()) {
						break;
					}
					this->_cached.insert(std::make_pair(peer->id(), peer));
				}
				updated = true;
			} while (false);
			uv_rwlock_rdunlock(&this->_lock);

			return ICN_RESULT_SUCCESS;
		}

		std::shared_ptr<Info> Store::local() {
			uv_rwlock_rdlock(&this->_localLock);
			std::shared_ptr<Info> localPeer = this->_localPeer;
			uv_rwlock_rdunlock(&this->_localLock);
			return localPeer;
		}

		uint32_t Store::updateLocal(std::shared_ptr<Info> localPeer) {
			std::shared_ptr<Info> old = NULL;
			uv_rwlock_rdlock(&this->_localLock);
			old = this->_localPeer;
			this->_localPeer = localPeer;
			uv_rwlock_rdunlock(&this->_localLock);
			return ICN_RESULT_SUCCESS;
		}
	}
}