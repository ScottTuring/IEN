#include <vector>
#include "../stack.h"
#include "../protocol/protocol.h"
#include "./udp_interface.h"

namespace ien {
	namespace net {
		uint32_t UdpInterface::box(
			const protocol::Package** packages,
			size_t count,
			const protocol::Package* refPackage,
			const uint8_t aesKey[crypto::aesBytes],
			uint8_t* buffer,
			size_t* inoutLength) {
			// 前边填充key hash
			history::KeyManager::calcKeyHash(aesKey, buffer);

			size_t dataLen = 0;
			size_t remainLen = *inoutLength - crypto::mixHashBytes;
			uint32_t result = protocol::encodePackages(packages, count, refPackage, buffer + crypto::mixHashBytes, remainLen, &dataLen);
			if (result) {
				return result;
			}

			int targetLen = crypto::aesEncryptDataWithPadding(aesKey, buffer + crypto::mixHashBytes, dataLen, remainLen, buffer + crypto::mixHashBytes, remainLen);
			if (targetLen < 0) {
				return ICN_RESULT_FAILED;
			}

			*inoutLength = (size_t)targetLen + crypto::mixHashBytes;
			return ICN_RESULT_SUCCESS;
		}

		uint32_t UdpInterface::box(
			Stack* stack,
			const protocol::Package** packages,
			size_t count,
			const protocol::Package* refPackage,
			const uint8_t aesKey[crypto::aesBytes],
			const peer::Const* remote,
			uint8_t* buffer,
			size_t* inoutLength) {
			size_t olen = 0;
			int ret = crypto::rsaEncrypt(
				aesKey,
				crypto::aesBytes,
				(const uint8_t*)&remote->publicKey,
				peer::publicKeyBytes(remote->publicKeyType),
				buffer,
				&olen,
				*inoutLength);
			if (ret) {
				return ret;
			}
			size_t used = olen;
			//write key hash:
			crypto::hashAesKey(aesKey, buffer + used);
			used += 8;
			size_t headlen = used;

			// 第一个包一定是ExchangeKeyPackage, 这里用secret签名
			protocol::ExchangeKey* pkg = (protocol::ExchangeKey*)packages[0];
			uint8_t buf4sign[sizeof(pkg->seq) + crypto::aesBytes];
			memcpy(buf4sign, (const uint8_t*)(&(pkg->seq)), sizeof(pkg->seq));
			memcpy(buf4sign + sizeof(pkg->seq), aesKey, crypto::aesBytes);

			uint8_t sign[crypto::signatureBytes];
			size_t signlen = crypto::signatureMaxBytes;
			ret = crypto::rsaSignMd5(buf4sign, sizeof(buf4sign), (const uint8_t*)&stack->secret().secret, stack->secret().secretLength, sign, &signlen);
			if (ret) {
				return ret;
			}

			memcpy(pkg->seqAndKeySign, sign, sizeof(pkg->seqAndKeySign));

			size_t outLen = 0;
			size_t remainLen = *inoutLength - headlen;
			uint32_t result = protocol::encodePackages(packages, count, refPackage, buffer + used, remainLen, &outLen);
			if (result) {
				return result;
			}

			int targetLen = crypto::aesEncryptDataWithPadding(aesKey, buffer + headlen, outLen, remainLen, buffer + headlen, remainLen);
			if (targetLen < 0) {
				return ICN_RESULT_FAILED;
			}

			*inoutLength = headlen + targetLen;
			return ICN_RESULT_SUCCESS;
		}

		uint32_t UdpInterface::unbox(
			Stack* stack,
			const uint8_t* data,
			size_t bytes,
			const protocol::Package* refPackage,
			protocol::Package** outPackages,
			uint8_t outKey[crypto::aesBytes],
			peer::Id* outKeyPeerid,
			bool* outIsEncrypto,
			bool* outHasExchange) {

			*outIsEncrypto = true;
			*outHasExchange = false;

			TlsEncodeBuffer* tlsData = stack->tlsEncodeBuffer();
			//todo: do independent verify every package
			if (bytes < protocol::minPackageLength) {
				return ICN_RESULT_FAILED;
			}

			if (data[0] == protocol::udpMagic[0]
				&& data[1] == protocol::udpMagic[1]) {
				int r = protocol::decodePackages(
					data + sizeof(protocol::udpMagic),
					bytes - sizeof(protocol::udpMagic),
					refPackage,
					outPackages,
					false);

				if (r == 0) {
					*outIsEncrypto = false;
					return ICN_RESULT_SUCCESS;
				}
			}


			//read mix hash
			if (bytes < protocol::minCryptoPackageLength) {
				return ICN_RESULT_FAILED;
			}
			size_t headerPos = crypto::mixHashBytes;
			bool hasExchange = stack->keyManager()->findByMixHash(data, outKey, outKeyPeerid, true, true) != ICN_RESULT_SUCCESS;
			if (hasExchange) {
				// 解出AESKey
				size_t olen = 0;
				size_t inLen = 0;
				int r = crypto::rsaDecrypt(
					data,
					(const uint8_t*)&stack->secret().secret,
					stack->secret().secretLength,
					outKey,
					&olen,
					crypto::aesBytes,
					&inLen);
				if (r == 0) {
					headerPos = inLen;
				}
				else {
					return ICN_RESULT_FAILED;
				}

				//先读keyhash
				const uint8_t* keyHash = data + inLen;
				headerPos += 8;
				//验证keyhash.
				if (!crypto::verifyAesKeyHash(outKey, keyHash)) {
					return ICN_RESULT_FAILED;
				}
			}

			int datalen = crypto::aesDecryptDataWithPadding(
				outKey,
				data + headerPos,
				bytes - headerPos,
				tlsData->udpDecryptBuffer,
				sizeof(tlsData->udpDecryptBuffer));

			if (datalen > 0) {
				int r = protocol::decodePackages(
					tlsData->udpDecryptBuffer,
					datalen,
					refPackage,
					outPackages,
					hasExchange);

				if (r == 0) {
					if (hasExchange) {
						protocol::ExchangeKey* pExchangeKey = (protocol::ExchangeKey*)outPackages[0];
						// TODO: 检查一下seqkeysign
						//将key加入keymananger,TODO:这里key在栈上
						stack->keyManager()->addKey(outKey, pExchangeKey->peerInfo->id(), true);
						*outKeyPeerid = pExchangeKey->peerInfo->id();
						*outHasExchange = true;
					}
					return ICN_RESULT_SUCCESS;
				}
				else {
					return ICN_RESULT_FAILED;
				}
			}
			else {
				return ICN_RESULT_FAILED;
			}
		}

		UdpInterface::UdpInterface(Stack* stack, const endpoint::Endpoint& local, uv_udp_t socket)
			: _stack(stack)
			, _socket(socket)
			, _local(local) {

		}

		void UdpInterface::_onData(std::shared_ptr<UdpInterface> ref, const uint8_t* data, size_t bytes, const endpoint::Endpoint& remote) {
			protocol::Package* resultPackages[protocol::maxMergeLength] = { NULL };
			bool isEncrypto = true;
			bool hasExchange = false;
			uint8_t key[crypto::aesBytes];
			peer::Id keyPeerid;
			uint32_t result = unbox(
				this->_stack, 
				data,
				bytes,
				NULL, 
				resultPackages, 
				key,
				&keyPeerid,
				&isEncrypto,
				&hasExchange);
			if (result) {
				return;
			}
			const size_t firstIndex = hasExchange ? 1 : 0;
			const protocol::Package* firstPackage = resultPackages[firstIndex];
			if (!firstPackage) {
				return;
			}
			result = ICN_RESULT_SUCCESS;

			if (firstPackage->isSnPackage()) {
				for (size_t ix = firstIndex; ix < protocol::maxMergeLength; ++ix) {
					protocol::Package* package = resultPackages[ix];
					if (!package) {
						break;
					}
					bool handled = false;
					uint32_t result = this->_stack->snClient()->pushUdpPackage(
						package,
						remote,
						ref,
						isEncrypto ? key : NULL,
						&handled);
				}
			}
			else {
				std::shared_ptr<tunnel::TunnelContainer> container = this->_stack->tunnelManager()->container(keyPeerid);
				if (!container.get()) {
					if (firstPackage->cmdtype == protocol::SynTunnel::command) {
						container = this->_stack->tunnelManager()->createContainer(keyPeerid);
					}
				}
				assert(container);
				for (size_t ix = firstIndex; ix < protocol::maxMergeLength; ++ix) {
					protocol::Package* package = resultPackages[ix];
					bool handled = false;
					if (!package) {
						break;
					}
					result = container->pushUdpPackage(package, ref, remote, &handled);
					// if handled set, ignore rest packages
					if (result != ICN_RESULT_SUCCESS || handled) {
						break;
					}
					container->pushSessionPackage(package, &handled);
				}
			}

			for (size_t ix = 0; ix < protocol::maxMergeLength; ++ix) {
				if (resultPackages[ix]) {
					delete resultPackages[ix];
				}
			}
		}


		static void _recvAllocBuffer(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf) {
			buf->base = (char*)malloc(suggestedSize);
		#ifdef ICN_OS_WIN
			buf->len = (ULONG)suggestedSize;
		#else
			buf->len = (size_t)suggestedSize;
		#endif 
		}


		void UdpInterface::_onRecv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
			std::shared_ptr<UdpInterface> udp = *(std::shared_ptr<UdpInterface>*)handle->data;

			if (nread > 0 || (nread == 0 && addr)) {
				endpoint::Endpoint remote;
				remote.fromSockAddr(addr, endpoint::protocolUdp);
				udp->_onData(udp, (const uint8_t*)buf->base, nread, remote);
				((uv_buf_t*)buf)->base = NULL;
			}

			if (buf->base) {
				free(buf->base);
			}
		}

		std::shared_ptr<UdpInterface> UdpInterface::open(Stack* stack, const endpoint::Endpoint& local) {
			struct sockaddr_storage dest;
			int ret = local.toSockAddr((struct sockaddr*)&dest);
			if (ret != 0) {
				return std::shared_ptr<UdpInterface>(NULL);
			}
			uv_udp_t socket;
			uv_udp_init(uv_default_loop(), &socket);
			ret = uv_udp_bind(&socket, (struct sockaddr*)&dest, 0);
			if (ret != 0) {
				return std::shared_ptr<UdpInterface>(NULL);
			}
			std::shared_ptr<UdpInterface>* udp = new std::shared_ptr<UdpInterface>(new UdpInterface(stack, local, socket));
			socket.data = udp;
			uv_udp_recv_start(&socket, _recvAllocBuffer, _onRecv);
			
			return std::shared_ptr<UdpInterface>(*udp);
		}

		const endpoint::Endpoint& UdpInterface::local() const {
			return this->_local;
		}


		struct SendToOp {
			uv_udp_send_t req;
			std::shared_ptr<UdpInterface> udpInterface;
			const uint8_t* buffer;
			size_t size;
		};

		static void _onSendToComplete(uv_udp_send_t* req, int status) {
			SendToOp* op = (SendToOp*)req;
			if (op->buffer) {
				free((void*)op->buffer);
				op->buffer = NULL;
			}
			delete op;
		}

		uint32_t UdpInterface::sendTo(
			std::shared_ptr<UdpInterface> ref, 
			const endpoint::Endpoint& to, 
			const uint8_t* buffer, 
			size_t size) {
			struct sockaddr_storage dest;
			int ret = to.toSockAddr((struct sockaddr*)&dest);
			if (ret != 0) {
				return ret;
			}

			SendToOp* op = new SendToOp;
			op->udpInterface = ref;
			op->buffer = buffer;
			op->size = size;
			

			uv_buf_t wrbuf = uv_buf_init((char*)op->buffer, (unsigned int)op->size);

			ret = uv_udp_send(&op->req, &this->_socket, &wrbuf, 1, (struct sockaddr*)&dest, _onSendToComplete);
			if (ret != 0) {
				return -1;
			}

			return 0;
		}


	}
}