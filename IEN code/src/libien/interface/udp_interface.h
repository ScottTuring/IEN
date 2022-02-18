/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <uv.h>
#include <memory>
#include "../common/common.h"
#include "../base/base.h"
#include "../protocol/protocol.h"
namespace ien {
	namespace net {
		class UdpInterface {
		public:
			static const size_t mtu = 1472;
			static const size_t maxDataSize = 1600;
		public:
			static std::shared_ptr<UdpInterface> open(Stack* stack, const endpoint::Endpoint& local);
			const endpoint::Endpoint& local() const;
			uint32_t sendTo(
				std::shared_ptr<UdpInterface> ref, 
				const endpoint::Endpoint& to, 
				const uint8_t* buffer, 
				size_t size);

			static uint32_t box(
				const protocol::Package** packages,
				size_t count,
				const protocol::Package* refPackage,
				const uint8_t aesKey[crypto::aesBytes],
				uint8_t* buffer,
				size_t* inoutLength);

			static uint32_t box(
				Stack* stack,
				const protocol::Package** packages,
				size_t count,
				const protocol::Package* refPackage,
				const uint8_t aesKey[crypto::aesBytes],
				const peer::Const* remote,
				uint8_t* buffer,
				size_t* inoutLength);

			static uint32_t unbox(
				Stack* stack,
				const uint8_t* data,
				size_t bytes,
				const protocol::Package* refPackage,
				protocol::Package** outPackages,
				uint8_t outKey[crypto::aesBytes],
				peer::Id* outKeyPeerid,
				bool* outIsEncrypto,
				bool* outHasExchange);
		private:
			UdpInterface(Stack* _stack, const endpoint::Endpoint& local, uv_udp_t socket);
			static void _onRecv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);
			void _onData(std::shared_ptr<UdpInterface> ref, const uint8_t* data, size_t bytes, const endpoint::Endpoint& remote);
			Stack* _stack;
			uv_udp_t _socket;
			endpoint::Endpoint _local;
		};
	}
}