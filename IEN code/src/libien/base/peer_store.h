/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <map>
#include <uv.h>
#include "./peer_info.h"

namespace ien {
	namespace peer {
		class Store {
		public:
			Store();
			~Store();
			uint32_t addStatic(std::shared_ptr<Info> peer);
			uint32_t addCached(std::shared_ptr<Info> peer, uint64_t expire);
			std::shared_ptr<Info> local();
			uint32_t updateLocal(std::shared_ptr<Info> localPeer);	
			std::shared_ptr<Info> getCachedOrStatic(const Id& peerid);
			std::shared_ptr<Info> getStatic(const Id& peerid);
			std::shared_ptr<Info> getCached(const Id& peerid);
		private:
			uv_rwlock_t _localLock;
			std::shared_ptr<Info> _localPeer;

			uv_rwlock_t _lock;
			std::map<Id, std::shared_ptr<Info>> _static;
			std::map<Id, std::shared_ptr<Info>> _cached;
		};
	}
}