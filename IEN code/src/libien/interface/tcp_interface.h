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
#include <vector>
#include "../base/base.h"
#include "../common/common.h"
#include "../protocol/protocol.h"

namespace ien {
	namespace net {
		class TcpListener {
		public:
			static std::shared_ptr<TcpListener> open(Stack* stack, const endpoint::Endpoint& local);
			const endpoint::Endpoint& local() const;
			Stack* stack() const;
		private:
			TcpListener(Stack* stack, const endpoint::Endpoint& local, uv_tcp_t socket);
			static void _onAccept(uv_stream_t* server, int status);
			Stack* _stack;
			uv_tcp_t _socket;
			endpoint::Endpoint _local;
			int _backlog;
		};

		bool isPassive(const endpoint::Endpoint& local, const endpoint::Endpoint& remote);
		bool markPassive(endpoint::Endpoint& local, endpoint::Endpoint& remote, bool passive);

		enum TcpInterfaceState {
			IEN_TCP_INTERFACE_STATE_NONE = 0,
			IEN_TCP_INTERFACE_STATE_CONNECTING,
			IEN_TCP_INTERFACE_STATE_ESTABLISH,
			IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE,
			IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP,
			IEN_TCP_INTERFACE_STATE_PACKAGE,
			IEN_TCP_INTERFACE_STATE_STREAM,
			IEN_TCP_INTERFACE_STATE_STREAM_RECV_CLOSED,
			IEN_TCP_INTERFACE_STATE_STREAM_CLOSING,
			IEN_TCP_INTERFACE_STATE_CLOSED,
		};

		class TcpInterfaceOwner {
		public:
			virtual uint32_t onEstablish() = 0;
			virtual void onError(uint32_t error) = 0;
			virtual uint32_t onFirstResp(const protocol::Package& package) = 0;
			virtual uint32_t onPackage(const protocol::Package** packages) = 0;
			virtual uint32_t onDrain() = 0;
			virtual uint32_t onData(const uint8_t* data, size_t bytes) = 0;
			virtual void onClosed() = 0;
			virtual void onLastEvent() = 0;
		};


		struct TcpBox {
			uint8_t* data;
			size_t bytes;
		};

		struct TcpUnboxState {
			static const size_t cacheSize = 64 * 1024;
			static const size_t maxBoxes = 32;
			static const size_t firstPackageSize = 6;
			static const size_t firstMixHashSize = crypto::mixHashBytes + 2;
			static const size_t firstExchangeSize = 138;


			static const uint8_t IEN_TCP_UNBOX_STATE_READING_LENGTH = 0;
			static const uint8_t IEN_TCP_UNBOX_STATE_READING_BODY = 1;



			size_t cacheLength;
			uint8_t* cache;
			uint8_t* backCache;
			uint8_t cacheData[cacheSize];
			uint8_t backCacheData[cacheSize];
			bool isCrypto;
			size_t firstBlockSize;
			uint8_t state;
			uint16_t boxSize;
			size_t boxCount;

			void swap() {
				uint8_t* pTemp = this->cache;
				this->cache = this->backCache;
				this->backCache = pTemp;
			}

			void append(const uint8_t* data, size_t bytes) {
				memcpy(this->cache + this->cacheLength, data, bytes);
				this->cacheLength += bytes;
			}

			void fill(size_t targetLength, const uint8_t* data, size_t datalen, size_t* pCopysize) {
				size_t copysize = 0;
				if (datalen + this->cacheLength >= targetLength) {
					copysize = targetLength - this->cacheLength;
				}
				else {
					copysize = datalen;
				}
				this->append(data, copysize);
				*pCopysize = copysize;
				return;
			}
		};

		class TcpInterface {
		public:
			struct Config {
				uint32_t firstPackageWaitTime;
			};
			const static size_t maxDataSize = 1600;
		public:
			static std::shared_ptr<TcpInterface> create(
				Stack* stack, 
				endpoint::Endpoint local, 
				endpoint::Endpoint remote);
			static std::shared_ptr<TcpInterface> wrap(
				Stack* stack, 
				uv_tcp_t socket, 
				endpoint::Endpoint local, 
				endpoint::Endpoint remote);
			
			inline const Config& config() const {
				return this->_config;
			}

			std::shared_ptr<TcpInterfaceOwner> owner();
			uint32_t setOwner(std::shared_ptr<TcpInterfaceOwner> owner);
			const endpoint::Endpoint& local() const;
			const endpoint::Endpoint& remote() const;
			TcpInterfaceState state() const;
			TcpInterfaceState setState(TcpInterfaceState oldState, TcpInterfaceState newState);
			void setKey(const uint8_t key[crypto::aesBytes]);
			uint32_t connect();
			uint32_t send(uint8_t* data, size_t len, size_t* outSent);
			uint32_t close();
			uint32_t reset();
			uint32_t pause();
			uint32_t resume();
		private:
			TcpInterface(Stack* stack,
				uv_tcp_t socket,
				endpoint::Endpoint local,
				endpoint::Endpoint remote);
			static void _onConnect(uv_connect_t* req, int status);
			static void _onRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
			static void _onClose(uv_handle_t* handle);
			static void _onSent(uv_write_t* req, int status);
			void _onRemoteClosed();
			void _onData(std::shared_ptr<TcpInterface> ref, const uint8_t* data, size_t bytes);
			int _unboxProc(const uint8_t* data, size_t bytes, TcpBox* pBoxes, size_t* pBoxesLen, size_t* pUsed, peer::Id* pRemoteId, bool* pHasExchange);
		private:
			Stack* _stack;
			Config _config;
			uv_tcp_t _socket;
			endpoint::Endpoint _local;
			endpoint::Endpoint _remote;
			
			uv_rwlock_t _stateLock;
			TcpInterfaceState _state;
			std::shared_ptr<TcpInterfaceOwner> _owner;

			volatile int32_t _pendingReqs;
			volatile int32_t _pendingBytes;
			int32_t _highWaterMark;
			int32_t _drainWaterMark;
			volatile int32_t _needDrain;

			uint8_t _key[crypto::aesBytes];
			TcpUnboxState _unboxState;
		};
	}
}