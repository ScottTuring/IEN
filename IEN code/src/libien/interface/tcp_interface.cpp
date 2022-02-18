#include "../thread/timer.h"
#include "../stack.h"
#include "./tcp_interface.h"

namespace ien {
    namespace net {
        bool isPassive(const endpoint::Endpoint& local, const endpoint::Endpoint& remote) {
            return !!local.port;
        }

        bool markPassive(endpoint::Endpoint& local, endpoint::Endpoint& remote, bool passive) {
            local.port = passive ? local.port : 0;
            remote.port = passive ? 0 : remote.port;
            return true;
        }

        TcpInterface::TcpInterface(
            Stack* stack,
            uv_tcp_t socket,
            endpoint::Endpoint local,
            endpoint::Endpoint remote)
            : _stack(stack)
            , _local(local)
            , _remote(remote)
            , _socket(socket)
            , _needDrain(0)
            , _owner(NULL)
            , _state(IEN_TCP_INTERFACE_STATE_NONE)
            , _highWaterMark(1024 * 1024 * 8)
            , _drainWaterMark(1024 * 1024 * 8 / 2) {
            uv_rwlock_init(&this->_stateLock);
        }

        std::shared_ptr<TcpInterface> TcpInterface::wrap(Stack* stack, uv_tcp_t socket, endpoint::Endpoint local, endpoint::Endpoint remote) {
            TcpInterface* inner = new TcpInterface(stack, socket, local, remote);
            std::shared_ptr<TcpInterface>* tcpInterface = new std::shared_ptr<TcpInterface>(inner);
            socket.data = tcpInterface;
            uv_tcp_init(uv_default_loop(), &socket);
            return *tcpInterface;
        }

        std::shared_ptr<TcpInterface> TcpInterface::create(Stack* stack, endpoint::Endpoint local, endpoint::Endpoint remote) {
            uv_tcp_t socket;
            return TcpInterface::wrap(stack, socket, local, remote);
        }

        uint32_t TcpInterface::setOwner(std::shared_ptr<TcpInterfaceOwner> owner) {
            std::shared_ptr<TcpInterfaceOwner> old = NULL;
            uint32_t result = ICN_RESULT_SUCCESS;

            uv_rwlock_rdlock(&this->_stateLock);
            do {
                if (this->_state == IEN_TCP_INTERFACE_STATE_CLOSED) {
                    result = ICN_RESULT_INVALID_STATE;
                    break;
                }
                old = this->_owner;
                this->_owner = owner;
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (result != ICN_RESULT_SUCCESS) {
                return result;
            }
            return ICN_RESULT_SUCCESS;
        }


        std::shared_ptr<TcpInterfaceOwner> TcpInterface::owner() {
            std::shared_ptr<TcpInterfaceOwner> owner = NULL;

            uv_rwlock_rdlock(&this->_stateLock);
            owner = this->_owner;
            uv_rwlock_rdunlock(&this->_stateLock);

            return owner;
        }


        const endpoint::Endpoint& TcpInterface::local() const {
            return this->_local;
        }

        const endpoint::Endpoint& TcpInterface::remote() const {
            return this->_remote;
        }

        TcpInterfaceState TcpInterface::state() const {
            return this->_state;
        }


        static void _readAllocBuffer(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf) {
            buf->base = (char*)malloc(suggestedSize);
#ifdef ICN_OS_WIN
            buf->len = (ULONG)suggestedSize;
#else
            buf->len = (size_t)suggestedSize;
#endif 
        }

        TcpInterfaceState TcpInterface::setState(TcpInterfaceState oldState, TcpInterfaceState newState) {
            TcpInterfaceState fromState = IEN_TCP_INTERFACE_STATE_NONE;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                if ((fromState = this->_state) != oldState) {
                    break;
                }
                this->_state = newState;
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (oldState != fromState) {
                return fromState;
            }

            if (newState == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP
                || newState == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE) {
                int ret = uv_read_start((uv_stream_t*)&this->_socket, _readAllocBuffer, _onRead);
            }
            return fromState;
        }


        void TcpInterface::_onRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
            std::shared_ptr<TcpInterface> tcpInterface = *(std::shared_ptr<TcpInterface>*)client->data;
            assert(tcpInterface);

            if (nread > 0) {
                tcpInterface->_onData(tcpInterface, (const uint8_t*)buf->base, nread);
            }
            else if (nread < 0) {
                if (nread == UV_EOF) {
                    // 对端关闭
                    tcpInterface->_onRemoteClosed();
                }
                else {
                    // 出错
                    tcpInterface->owner()->onError(ICN_RESULT_FAILED);
                }
            }
            else {
                // 不做处理
                // nread might be 0, which does not indicate an error or EOF. This is equivalent to EAGAIN or EWOULDBLOCK under read(2).
            }

            if (buf->base) {
                free(buf->base);
            }
        }

        void TcpInterface::setKey(const uint8_t key[crypto::aesBytes]) {
            memcpy(this->_key, key, crypto::aesBytes);
        }

        void TcpInterface::_onConnect(uv_connect_t* req, int status) {
            std::shared_ptr<TcpInterface>* tcpInterface = (std::shared_ptr<TcpInterface>*)req->handle->data;
            assert(tcpInterface);

            delete req;

            // 触发回调
            if (status == 0) {
                (*tcpInterface)->owner()->onEstablish();
            }
            else {
                (*tcpInterface)->owner()->onError(ICN_RESULT_FAILED);
            }
        }

        uint32_t TcpInterface::connect() {
            uint32_t result = ICN_RESULT_SUCCESS;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                if (IEN_TCP_INTERFACE_STATE_NONE != this->_state) {
                    result = ICN_RESULT_INVALID_STATE;
                    break;
                }
                this->_state = IEN_TCP_INTERFACE_STATE_CONNECTING;
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (result != ICN_RESULT_SUCCESS) {
                return result;
            }

            struct sockaddr_storage addr;
            int ret = this->_local.toSockAddr((struct sockaddr*)&addr);
            if (ret != 0) {
                return ret;
            }

            struct sockaddr_storage dest;
            ret = this->_remote.toSockAddr((struct sockaddr*)&dest);
            if (ret != 0) {
                return ret;
            }

            uv_connect_t* connect = new uv_connect_t;
            ret = uv_tcp_connect(connect, &this->_socket, (const struct sockaddr*)&dest, _onConnect);
            if (ret != 0) {
                return -1;
            }

            return result;
        }


        struct WriteOp {
            uv_write_t req;
            std::shared_ptr<TcpInterface> tcpInterface;
            const uint8_t* buffer;
            size_t size;

            WriteOp(std::shared_ptr<TcpInterface> tcpInterface, const uint8_t* buffer, size_t size)
                : tcpInterface(tcpInterface)
                , buffer(buffer)
                , size(size) {

            }

            ~WriteOp() {
                if (this->buffer) {
                    free((void*)this->buffer);
                }
            }

        };


        int TcpInterface::_unboxProc(
            const uint8_t* data, 
            size_t bytes, 
            TcpBox* pBoxes, 
            size_t* pBoxesLen, 
            size_t* pUsed, 
            peer::Id* pRemoteId, 
            bool* pHasExchange) {

            if (bytes == 0) {
                *pBoxesLen = 0;
                return ICN_RESULT_SUCCESS;
            }

            int r = 0;
            const uint8_t* pData = NULL;
            size_t boxCount = 0;
            size_t dataLen = 0;
            size_t readed = 0;

            switch (this->_state) {
            case IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE:
                if (this->_unboxState.state == TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_LENGTH) {
                    if (this->_unboxState.cacheLength > 0) {
                        size_t copysize = 0;
                        this->_unboxState.fill(this->_unboxState.firstBlockSize, data, bytes, &copysize);

                        readed = copysize;
                        if (this->_unboxState.cacheLength == this->_unboxState.firstBlockSize) {
                            pData = this->_unboxState.cache;
                        }
                        else {
                            *pBoxesLen = boxCount;
                            return ICN_RESULT_SUCCESS;
                        }
                    }
                    else {
                        if (bytes < this->_unboxState.firstBlockSize) {
                            this->_unboxState.append(data, bytes);
                            *pBoxesLen = boxCount;
                            return ICN_RESULT_SUCCESS;
                        }

                        pData = data;
                    }

                    //start guess
                    if (pData) {
                        bool decodeOK = false;

                        if (this->_unboxState.firstBlockSize == TcpUnboxState::firstPackageSize) {
                            //继续
                            this->_unboxState.firstBlockSize = TcpUnboxState::firstMixHashSize;
                            if (pData == this->_unboxState.cache) {
                                *pUsed += readed;
                                return this->_unboxProc(data + readed, bytes - readed, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                            }
                            else {
                                return this->_unboxProc(data, bytes, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                            }
                        }
                        else if (this->_unboxState.firstBlockSize == TcpUnboxState::firstMixHashSize) {
                            //看看是不是aeskey
                            if (this->_stack->keyManager()->findByMixHash(data, this->_key, pRemoteId, true, true) == ICN_RESULT_SUCCESS) {
                                //aeskeyAES Unbox,先解一个TCP_CRYPTO_BOX 加密的2字节长度
                                BufferStream bufferStream;
                                bufferStream.init((uint8_t*)(pData + crypto::mixHashBytes), TcpUnboxState::firstExchangeSize - crypto::mixHashBytes);
                                uint16_t aesBoxLen = 0;
                                bufferStream.readUInt16(&aesBoxLen);
                                this->_unboxState.boxSize = aesBoxLen;

                                //重新调整cache
                                readed = TcpUnboxState::firstMixHashSize;
                                this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_BODY;
                                *pUsed += readed;
                                if (pData == this->_unboxState.cache) {
                                    size_t cacheReaded = TcpUnboxState::firstMixHashSize;
                                    memcpy(this->_unboxState.cache, this->_unboxState.cache + cacheReaded, this->_unboxState.cacheLength - cacheReaded);
                                    this->_unboxState.cacheLength -= cacheReaded;
                                    *pUsed += readed;
                                    return this->_unboxProc(data + readed, bytes - readed, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                                }
                                readed = TcpUnboxState::firstMixHashSize;
                                *pUsed += readed;
                                return this->_unboxProc(data + readed, bytes - readed, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                            }

                            this->_unboxState.firstBlockSize = TcpUnboxState::firstExchangeSize;
                            if (pData == this->_unboxState.cache) {
                                *pUsed += readed;
                                return this->_unboxProc(data + readed, bytes - readed, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                            }
                            else {
                                return this->_unboxProc(data, bytes, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                            }
                        }
                        else if (this->_unboxState.firstBlockSize == TcpUnboxState::firstExchangeSize) {
                            const peer::Secret* secret = &this->_stack->secret();
                            assert(secret);

                            TlsEncodeBuffer* tlsData = this->_stack->tlsEncodeBuffer();


                            // 解出AESKey
                            size_t olen = 0;
                            size_t inLen = 0;
                            r = crypto::rsaDecrypt(
                                pData, 
                                (uint8_t*)(secret->secret.rsa1024), 
                                secret->secretLength, 
                                this->_key, 
                                &olen, 
                                crypto::aesBytes, 
                                &inLen);
                            if (r == 0) {
                                BufferStream bufferStream;
                                bufferStream.init((uint8_t*)(pData + inLen), TcpUnboxState::firstExchangeSize - inLen);
                                //先读keyhash
                                uint8_t keyHash[8];
                                bufferStream.readByteArray(keyHash, 8);
                                //验证keyhash.
                                if (crypto::verifyAesKeyHash(this->_key, keyHash)) {
                                    uint16_t aesBoxLen = 0;
                                    bufferStream.readUInt16(&aesBoxLen);
                                    this->_unboxState.boxSize = aesBoxLen;

                                    *pHasExchange = true;
                                    this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_BODY;
                                    if (pData == this->_unboxState.cache) {
                                        //重新调整cache
                                        size_t cacheReaded = olen + 8 + 2;
                                        memcpy(this->_unboxState.cache, this->_unboxState.cache + cacheReaded, this->_unboxState.cacheLength - cacheReaded);
                                        this->_unboxState.cacheLength -= cacheReaded;
                                        *pUsed += readed;
                                        return this->_unboxProc(data + readed, bytes - readed, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                                    }
                                    readed = inLen + 8 + 2;
                                    *pUsed += readed;
                                    return this->_unboxProc(data + readed, bytes - readed, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                                }
                            }
                            else {
                                *pBoxesLen = boxCount;
                                return ICN_RESULT_INVALID_FORMAT;
                            }
                        }
                    }
                }
                else if (this->_unboxState.state == TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_BODY) {
                    if (this->_unboxState.cacheLength != 0) {
                        //cache存在 
                        //TODO：从性能测试的角度考虑，这里最好统计一下发生了多少次数据copy
                        size_t copysize = 0;
                        this->_unboxState.fill(this->_unboxState.boxSize, data, bytes, &copysize);

                        if (this->_unboxState.cacheLength == this->_unboxState.boxSize) {
                            this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_LENGTH;
                            pBoxes->data = this->_unboxState.cache;
                            pBoxes->bytes = this->_unboxState.cacheLength;
                            boxCount++;
                            this->_unboxState.cacheLength = 0;

                            this->_unboxState.swap();
                        }
                        *pBoxesLen = boxCount;
                        (*pUsed) += copysize;
                        return ICN_RESULT_SUCCESS;
                    }
                    else {
                        if (bytes >= this->_unboxState.boxSize) {
                            this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_LENGTH;
                            pBoxes->data = (uint8_t*)data;
                            pBoxes->bytes = this->_unboxState.boxSize;
                            boxCount++;
                            *pBoxesLen = boxCount;
                            (*pUsed) += this->_unboxState.boxSize;
                            return ICN_RESULT_SUCCESS;
                        }
                        else {

                            this->_unboxState.append(data, bytes);
                            (*pUsed) += bytes;
                            *pBoxesLen = boxCount;
                            return ICN_RESULT_SUCCESS;
                        }
                    }
                }
                else {
                    assert(0);
                }

                break;
            case IEN_TCP_INTERFACE_STATE_PACKAGE:
            case IEN_TCP_INTERFACE_STATE_STREAM:
            case IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP:
                if (this->_unboxState.state == TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_LENGTH) {
                    //cache存在
                    if (this->_unboxState.cacheLength > 0) {
                        assert(this->_unboxState.cacheLength == 1);
                        this->_unboxState.cache[1] = data[0];
                        this->_unboxState.boxSize = *((uint16_t*)(this->_unboxState.cache));//TODO:没有正确处理字节序

                        this->_unboxState.boxSize = this->_unboxState.boxSize;
                        this->_unboxState.cacheLength = 0;
                        this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_BODY;
                        (*pUsed) += 1;

                        return this->_unboxProc(data + 1, bytes - 1, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                    }
                    else {
                        if (bytes > 1) {
                            this->_unboxState.boxSize = *((uint16_t*)(data));//TODO:没有正确处理字节序
                            this->_unboxState.boxSize = this->_unboxState.boxSize;
                            this->_unboxState.cacheLength = 0;
                            this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_BODY;
                            (*pUsed) += 2;
                            return this->_unboxProc(data + 2, bytes - 2, pBoxes, pBoxesLen, pUsed, pRemoteId, pHasExchange);
                        }
                        else {
                            (*pUsed) += 1;
                            this->_unboxState.append(data, bytes);
                            *pBoxesLen = boxCount;
                            return ICN_RESULT_SUCCESS;
                        }
                    }
                }
                else if (this->_unboxState.state == TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_BODY) {
                    if (this->_unboxState.cacheLength != 0) {
                        //cache存在
                        //TODO：从性能测试的角度考虑，这里最好统计一下发生了多少次数据copy
                        size_t copysize = 0;
                        this->_unboxState.fill(this->_unboxState.boxSize, data, bytes, &copysize);
                        (*pUsed) += copysize;
                        if (this->_unboxState.cacheLength >= this->_unboxState.boxSize) {
                            this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_LENGTH;
                            pBoxes->data = this->_unboxState.cache;
                            pBoxes->bytes = this->_unboxState.cacheLength;
                            boxCount++;
                            this->_unboxState.cacheLength = 0;

                            this->_unboxState.swap();
                            if (this->_state == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP) {
                                *pBoxesLen = boxCount;
                                return ICN_RESULT_SUCCESS;
                            }
                            if (boxCount < *pBoxesLen) {
                                size_t leftBoxLen = *pBoxesLen - boxCount;
                                int ret = this->_unboxProc(data + copysize, bytes - copysize, pBoxes + 1, &leftBoxLen, pUsed, pRemoteId, pHasExchange);
                                boxCount += leftBoxLen;
                                *pBoxesLen = boxCount;
                                return ret;
                            }
                        }
                        *pBoxesLen = boxCount;
                        return ICN_RESULT_SUCCESS;
                    }
                    else {
                        if (bytes >= this->_unboxState.boxSize) {
                            this->_unboxState.state = TcpUnboxState::IEN_TCP_UNBOX_STATE_READING_LENGTH;
                            pBoxes->data = (uint8_t*)data;
                            pBoxes->bytes = this->_unboxState.boxSize;
                            boxCount++;
                            (*pUsed) += this->_unboxState.boxSize;
                            if (this->_state == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP) {
                                *pBoxesLen = boxCount;
                                return ICN_RESULT_SUCCESS;
                            }
                            if (boxCount < *pBoxesLen) {
                                size_t leftBoxLen = *pBoxesLen - boxCount;
                                int ret = this->_unboxProc(data + this->_unboxState.boxSize, bytes - this->_unboxState.boxSize, pBoxes + 1, &leftBoxLen, pUsed, pRemoteId, pHasExchange);
                                boxCount += leftBoxLen;
                                *pBoxesLen = boxCount;
                                return ret;
                            }
                        }
                        else {
                            (*pUsed) += bytes;
                            this->_unboxState.append(data, bytes);
                            *pBoxesLen = boxCount;
                            return ICN_RESULT_SUCCESS;
                        }
                    }
                }
                else {
                    assert(0);
                    *pBoxesLen = boxCount;
                    return ICN_RESULT_INVALID_STATE;
                }

                break;
            }

            *pBoxesLen = boxCount;
            return ICN_RESULT_SUCCESS;
        }

        void TcpInterface::_onData(std::shared_ptr<TcpInterface> ref, const uint8_t* data, size_t bytes) {
            TcpBox boxes[TcpUnboxState::maxBoxes];
            size_t boxesLen = TcpUnboxState::maxBoxes;
            size_t used = 0;
            peer::Id remoteId;
            bool hasExchange = false;
            int r = this->_unboxProc(
                data,
                bytes,
                boxes,
                &boxesLen,
                &used,
                &remoteId, 
                &hasExchange);

            if (r != ICN_RESULT_SUCCESS) {
                this->reset();
                return;
            }

            if (boxesLen < 1) {
                return;
            }

            if (this->_state == IEN_TCP_INTERFACE_STATE_STREAM) {
                for (size_t i = 0; i < boxesLen; ++i) {
                    TcpBox* pBox = boxes + i;
                    r = crypto::aesDecryptDataWithPadding(
                        this->_key,
                        pBox->data,
                        pBox->bytes,
                        pBox->data,
                        pBox->bytes);
                    if (r <= 0) {
                        this->reset();
                        return;
                    }
                    this->owner()->onData(pBox->data, r);
                }
            }
            else if (this->_state == IEN_TCP_INTERFACE_STATE_PACKAGE) {
                protocol::Package* resultPackages[protocol::maxMergeLength] = { NULL };
                for (size_t i = 0; i < boxesLen; ++i) {
                    TcpBox* pBox = boxes + i;
                    size_t realSize = pBox->bytes;
                    r = crypto::aesDecryptDataWithPadding(
                        this->_key,
                        pBox->data,
                        pBox->bytes,
                        pBox->data,
                        pBox->bytes);
                    if (r <= 0) {
                        this->reset();
                        return;
                    }
                    realSize = r;

                    r = protocol::decodePackages(
                        pBox->data,
                        realSize,
                        NULL,
                        resultPackages,
                        false);
                    if (r <= 0) {
                        this->reset();
                        return;
                    }
                    if (this->_state == IEN_TCP_INTERFACE_STATE_PACKAGE) {
                        r = this->owner()->onPackage((const protocol::Package**)resultPackages);
                    }
                    
                    for (size_t ix = 0; ix < protocol::maxMergeLength; ++ix) {
                        if (resultPackages[ix]) {
                            delete resultPackages[ix];
                        }
                    }
                }
            }
            else if (this->_state == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE
                || this->_state == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP) {
                assert(boxesLen == 1);
                TcpBox* pBox = boxes;
                size_t realSize = pBox->bytes;
                r = crypto::aesDecryptDataWithPadding(
                    this->_key,
                    pBox->data,
                    pBox->bytes,
                    pBox->data,
                    pBox->bytes);
                if (r <= 0) {
                    this->reset();
                    return;
                }
                realSize = r;
                protocol::Package* packages[protocol::maxMergeLength] = { NULL };
                r = protocol::decodePackages(
                    pBox->data,
                    realSize,
                    NULL,
                    packages,
                    hasExchange);
                if (r <= 0) {
                    this->reset();
                    return;
                }
                if (this->_state == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE) {
                    const protocol::Package* firstPackage = NULL;
                    if (packages[0]->cmdtype == protocol::ExchangeKey::command) {
                        protocol::ExchangeKey* exchange = (protocol::ExchangeKey*)packages[0];
                        firstPackage = packages[1];
                        assert(!packages[2]);
                        this->_stack->keyManager()->addKey(this->_key, exchange->fromPeerid, true);
                    }
                    else {
                        firstPackage = packages[0];
                        assert(!packages[1]);
                    }
                    assert(firstPackage);

                    std::shared_ptr<tunnel::TunnelContainer> tunnel = this->_stack->tunnelManager()->container(remoteId);
                    if (!tunnel.get()) {
                        if (firstPackage->cmdtype == protocol::SynTunnel::command
                            || firstPackage->cmdtype == protocol::TcpSynConnection::command) {
                            tunnel = this->_stack->tunnelManager()->createContainer(remoteId);
                        }
                    }
                    if (!tunnel.get()) {
                        r = ICN_RESULT_INVALID_PARAM;
                    }
                    else {
                        r = tunnel->pushTcpFirstPackage(firstPackage, ref);
                    }
                }
                else if (this->_state == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_RESP) {
                    const protocol::Package* firstPackage = packages[0];
                    assert(!packages[1]);
                    assert(firstPackage);
                    r = this->owner()->onFirstResp(*firstPackage);
                }
                for (size_t ix = 0; ix < protocol::maxMergeLength; ++ix) {
                    if (packages[ix]) {
                        delete packages[ix];
                    }
                }

                if (r != ICN_RESULT_SUCCESS) {
                    return;
                }
            }
            else {
                assert(0);
                return;
            }

            if (used < bytes) {
                //TODO:注意这个递归处理
                this->_onData(ref, data + used, bytes - used);
            }
            return;
        }

        void TcpInterface::_onRemoteClosed() {
            TcpInterfaceState oldState = IEN_TCP_INTERFACE_STATE_NONE;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                oldState = this->_state;
                if (oldState == IEN_TCP_INTERFACE_STATE_STREAM) {
                    this->_state = IEN_TCP_INTERFACE_STATE_STREAM_RECV_CLOSED;
                    break;
                }
                else if (oldState == IEN_TCP_INTERFACE_STATE_STREAM_CLOSING) {
                    this->_state = IEN_TCP_INTERFACE_STATE_CLOSED;
                    this->_owner = NULL;
                }
                else {
                    // do nothing wait send error
                }
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (oldState == IEN_TCP_INTERFACE_STATE_STREAM) {
                // remote closed
                this->owner()->onData(NULL, 0);
            }
        }


        void TcpInterface::_onSent(uv_write_t* req, int status) {
            WriteOp* op = (WriteOp*)req;

            if (status != 0) {
                op->tcpInterface->owner()->onError(ICN_RESULT_FAILED);
            }

            IcnAtomicDec32(&op->tcpInterface->_pendingReqs);

            int ret = 0;
            IcnAtomicAdd32(&op->tcpInterface->_pendingBytes, -(int32_t)op->size);

            if (op->tcpInterface->_needDrain
                && op->tcpInterface->_pendingBytes <= op->tcpInterface->_drainWaterMark
                && IcnAtomicCompareAndSwap32(&op->tcpInterface->_needDrain, 1, 0) == 1) {

                op->tcpInterface->owner()->onDrain();
            }

            delete op;
        }

        uint32_t TcpInterface::send(uint8_t* data, size_t len, size_t* outSent) {
            uint32_t result = ICN_RESULT_SUCCESS;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                if (this->_state == IEN_TCP_INTERFACE_STATE_CLOSED) {
                    result = ICN_RESULT_INVALID_STATE;
                    break;
                }
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (result != ICN_RESULT_SUCCESS) {
                return result;
            }

            int toSend = len;
            for (;;) {
                int cur = this->_pendingBytes;
                if (cur >= this->_highWaterMark) {
                    *outSent = 0;
                    return ICN_RESULT_PENDING;
                }

                int next = cur + toSend;
                if (next > this->_highWaterMark) {
                    toSend = this->_highWaterMark - cur;
                    next = this->_highWaterMark;

                    result = ICN_RESULT_PENDING;
                }

                int old = IcnAtomicCompareAndSwap32(&this->_pendingBytes, cur, next);
                if (old == cur) {
                    IcnAtomicInc32(&this->_pendingReqs);
                    break;
                }
            }

            if (result == ICN_RESULT_PENDING) {
                this->_needDrain = 1;
            }

            std::shared_ptr<TcpInterface>* tcpInterface = (std::shared_ptr<TcpInterface>*)((uv_handle_t*)&this->_socket)->data;
            WriteOp* op = new WriteOp(*tcpInterface, data, toSend);
            uv_buf_t wrbuf = uv_buf_init((char*)op->buffer, (unsigned int)op->size);
            int ret = uv_write(&op->req, (uv_stream_t*)&this->_socket, &wrbuf, 1, _onSent);
            if (ret != 0) {
                return ICN_RESULT_FAILED;
            }
            return result;
        }


        void TcpInterface::_onClose(uv_handle_t* handle) {
            std::shared_ptr<TcpInterface>* tcpInterface = (std::shared_ptr<TcpInterface>*)handle->data;
            handle->data = NULL;
            delete tcpInterface;
            // TODO change status
        }


        uint32_t TcpInterface::close() {
            TcpInterfaceState oldState = IEN_TCP_INTERFACE_STATE_NONE;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                oldState = this->_state;
                if (this->_state == IEN_TCP_INTERFACE_STATE_CLOSED) {
                    break;
                }
                if (this->_state == IEN_TCP_INTERFACE_STATE_STREAM) {
                    this->_state = IEN_TCP_INTERFACE_STATE_STREAM_CLOSING;
                    break;
                }
                this->_state = IEN_TCP_INTERFACE_STATE_CLOSED;
                this->_owner = NULL;
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (oldState == IEN_TCP_INTERFACE_STATE_CLOSED) {
                return ICN_RESULT_INVALID_STATE;
            }

            uv_close((uv_handle_t*)&this->_socket, _onClose);

            return ICN_RESULT_SUCCESS;
        }

        uint32_t TcpInterface::reset() {
            TcpInterfaceState oldState = IEN_TCP_INTERFACE_STATE_NONE;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                oldState = this->_state;
                if (this->_state == IEN_TCP_INTERFACE_STATE_CLOSED) {
                    break;
                }
                if (this->_state == IEN_TCP_INTERFACE_STATE_STREAM) {
                    this->_state = IEN_TCP_INTERFACE_STATE_STREAM_CLOSING;
                    break;
                }
                this->_state = IEN_TCP_INTERFACE_STATE_CLOSED;
                this->_owner = NULL;
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (oldState == IEN_TCP_INTERFACE_STATE_CLOSED) {
                return ICN_RESULT_INVALID_STATE;
            }

            uv_close((uv_handle_t*)&this->_socket, _onClose);

            return ICN_RESULT_SUCCESS;
        }

        uint32_t TcpInterface::pause() {
            uint32_t result = ICN_RESULT_SUCCESS;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                if (this->_state == IEN_TCP_INTERFACE_STATE_CLOSED) {
                    result = ICN_RESULT_INVALID_STATE;
                    break;
                }
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (result != ICN_RESULT_SUCCESS) {
                return result;
            }

            int ret = uv_read_stop((uv_stream_t*)&this->_socket);
            if (ret != 0) {
                return ICN_RESULT_FAILED;
            }
            return result;
        }

        uint32_t TcpInterface::resume() {
            uint32_t result = ICN_RESULT_SUCCESS;
            uv_rwlock_rdlock(&this->_stateLock);
            do {
                if (this->_state == IEN_TCP_INTERFACE_STATE_CLOSED) {
                    result = ICN_RESULT_INVALID_STATE;
                    break;
                }
            } while (false);
            uv_rwlock_rdunlock(&this->_stateLock);

            if (result != ICN_RESULT_SUCCESS)
            {
                return result;
            }

            int ret = uv_read_start((uv_stream_t*)&this->_socket, _readAllocBuffer, _onRead);
            if (ret != 0) {
                return ICN_RESULT_FAILED;
            }
            return result;
        }


        class WaitFirstPackageOp
            : TimerCallback {
        public:
            static void start(std::shared_ptr<TcpInterface> tcpInterface) {
                WaitFirstPackageOp* op = new WaitFirstPackageOp(tcpInterface);
                op->_timer.start(std::shared_ptr<TimerCallback>((TimerCallback*)op), tcpInterface->config().firstPackageWaitTime);
            }

            void onTimer() {
                if (this->_tcpInterface->state() == IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE) {
                    this->_tcpInterface->reset();
                }
            }

        private:
            WaitFirstPackageOp(std::shared_ptr<TcpInterface> tcpInterface)
                : _tcpInterface(tcpInterface)
                , _timer(Timer()) {
            }

            std::shared_ptr<TcpInterface> _tcpInterface;
            Timer _timer;
        };

        void TcpListener::_onAccept(uv_stream_t* server, int status) {
            if (status < 0) {
                return;
            }

            std::shared_ptr<TcpListener>* tcpListener = (std::shared_ptr<TcpListener>*)server->data;
            uv_tcp_t socket;
            uv_tcp_init(uv_default_loop(), &socket);

            int acceptRet = uv_accept(server, (uv_stream_t*)&socket);
            if (acceptRet != 0) {
                return;
            }

            endpoint::Endpoint local;
            // 保存地址到socket
            {
                struct sockaddr_storage addr = { 0 };
                int addrLen = sizeof(addr);
                int ret = uv_tcp_getsockname(&socket, (struct sockaddr*)&addr, &addrLen);
                if (ret != 0) {
                    return;
                }

                local.fromSockAddr((struct sockaddr*)&addr, endpoint::protocolTcp);
            }

            endpoint::Endpoint remote;
            {
                struct sockaddr_storage addr = { 0 };
                int addrLen = sizeof(addr);
                int ret = uv_tcp_getpeername(&socket, (struct sockaddr*)&addr, &addrLen);
                if (ret != 0) {
                    return;
                }

                remote.fromSockAddr((struct sockaddr*)&addr, endpoint::protocolTcp);
            }

            std::shared_ptr<TcpInterface> tcpInterface = TcpInterface::wrap((*tcpListener)->stack(), socket, local, remote);
            WaitFirstPackageOp::start(tcpInterface);
            tcpInterface->setState(IEN_TCP_INTERFACE_STATE_NONE, IEN_TCP_INTERFACE_STATE_WAIT_FIRST_PACKAGE);
        }

        Stack* TcpListener::stack() const {
            return this->_stack;
        }

        std::shared_ptr<TcpListener> TcpListener::open(Stack* stack, const endpoint::Endpoint& local) {
            struct sockaddr_storage addr;
            int ret = local.toSockAddr((struct sockaddr*)&addr);
            if (ret != 0) {
                return std::shared_ptr<TcpListener>(NULL);
            }

            uv_tcp_t socket;
            uv_tcp_init(uv_default_loop(), &socket);
            int bindRet = uv_tcp_bind(&socket, (const struct sockaddr*)&addr, 0);
            if (bindRet != 0) {
                return std::shared_ptr<TcpListener>(NULL);
            }
            int backlog = 128;

            std::shared_ptr<TcpListener> tcp(new TcpListener(stack, local, socket));
            socket.data = new std::shared_ptr<TcpListener>(tcp);
            ret = uv_listen((uv_stream_t*)&socket, backlog, _onAccept);
            if (ret != 0) {
                return std::shared_ptr<TcpListener>(NULL);
            }
            return tcp;
        }

        const endpoint::Endpoint& TcpListener::local() const {
            return this->_local;
        }

        TcpListener::TcpListener(Stack* stack, const endpoint::Endpoint& local, uv_tcp_t socket)
            : _stack(stack)
            , _local(local)
            , _socket(socket) {

        }
    }
}