/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <map>
#include <memory>
#include <uv.h>
#include "../common/common.h"
#include "../base/base.h"
#include "./tunnel_container.h"


namespace ien {
	namespace tunnel {
		class TunnelManager {
		public:
			TunnelManager(Stack* stack);
			~TunnelManager();
			std::shared_ptr<TunnelContainer> container(const peer::Id& remote);
			std::shared_ptr<TunnelContainer> createContainer(const peer::Id& remote);
			void removeContainer(const peer::Id& remote);
		private:
			Stack* _stack;
			uv_rwlock_t _mapLock;
			std::map<peer::Id, std::shared_ptr<TunnelContainer>> _containerEntry;
		};
	}
}