#include "../common/common.h"
#include "./crypto.h"
#include "./peer_info.h"

namespace ien {
	namespace peer {

		const static uint32_t keyMap[][3] = {
			{publicKeyBytesRsa1024, secretKeyMaxBytesRsa1024, 1024},
			{publicKeyBytesRsa2048, secretKeyMaxBytesRsa2048, 2048},
			{publicKeyBytesRsa3072, secretKeyMaxBytesRsa3072, 3072},
		};


		uint32_t publicKeyBytes(uint8_t keyType) {
			return keyMap[keyType][0];
		}

		uint32_t secretKeyMaxBytes(uint8_t keyType) {
			return keyMap[keyType][1];
		}

		uint32_t rsaBits(uint8_t keyType) {
			return keyMap[keyType][2];
		}


		int Id::compare(const Id* right) const {
			return memcmp(this, right, idBytes);
		}

		void Id::toString(char out[idChars]) const {
			for (size_t ix = 0; ix < idBytes; ++ix) {
				sprintf(out + ix * 2, "%02x", *((uint8_t*)this + ix));
			}
		}


		uint32_t Const::init(
			const Area* areaCode,
			const char* deviceId,
			uint8_t publicKeyType,
			Id* outPeerid,
			Secret* outSecret
		) {
			memset(this, 0, sizeof(Const));
			this->areaCode = *areaCode;
			memcpy(this->deviceId, deviceId, strlen(deviceId));
			this->publicKeyType = publicKeyType;

			size_t publicLen = publicKeyBytes(publicKeyType), secretLen = secretKeyMaxBytes(publicKeyType);
			uint32_t secretSize = rsaBits(publicKeyType);
			
			int ret = crypto::rsaGenerate(secretSize, (uint8_t*)this->publicKey.rsa1024, &publicLen, (uint8_t*)outSecret, &secretLen);
			if (ret) {
				return ret;
			}

			outSecret->secretLength = (uint16_t)secretLen;

			return ICN_RESULT_SUCCESS;
		}

		uint32_t Const::toId(Id* outPeerid) const {
			uint8_t hash[32];
			crypto::sha256Hash((const uint8_t*)&this->publicKey, publicKeyBytes(this->publicKeyType), hash);

			uint8_t* ptrPeerid = (uint8_t*)outPeerid;
			ptrPeerid[0] = this->areaCode.continent << 2 | this->areaCode.country >> 6;
			ptrPeerid[1] = this->areaCode.country << 2 | this->areaCode.carrier >> 2;
			ptrPeerid[2] = this->areaCode.carrier << 6 | this->areaCode.city >> 8;
			ptrPeerid[3] = this->areaCode.city << 8 >> 8;
			ptrPeerid[4] = this->areaCode.inner;

			for (int i = 0; i < 27; i++) {
				// 从第5个byte开始是hash部分
				ptrPeerid[5 + i] = hash[i];
			}

			return ICN_RESULT_SUCCESS;
		}

		uint32_t Const::verify(const Id* peerid) const {
			Id pid;
			this->toId(&pid);
			if (!peerid->compare(&pid)) {
				return ICN_RESULT_SUCCESS;
			}
			return ICN_RESULT_INVALID_PARAM;
		}

        Info::Info(Info* proto)
        : _flags(proto->_flags) 
        , _peerid(proto->_peerid) 
        , _endpoints(proto->_endpoints) 
        , _snList(proto->_snList) 
        , _snInfoList(proto->_snInfoList)
        , _createTime(proto->_createTime)
        , _secret(NULL) {
            if (proto->_secret) {
                this->_secret = new Secret();
                memcpy(this->_secret, proto->_secret, sizeof(Secret));
            }
            memcpy(this->_signature, this->_signature, sizeof(this->_signature));
        }

        Info::~Info() {
            if (this->_secret) {
                delete this->_secret;
            }
        }

		bool Info::isSigned() const {
			return (this->_flags & Info::flagSigned) != 0;
		}


        uint32_t Info::writeSignField(
            bool isWillResign,
            BufferStream& stream,
            size_t* writeBytes,
            size_t* signStartPos)
        {
            uint32_t ret = ICN_RESULT_SUCCESS;
            size_t startPos = stream.pos();
            uint8_t writeEndpointCount = (uint8_t)this->_endpoints.size();
            stream.writeUInt8(writeEndpointCount, writeBytes);
            if (!isWillResign) {
                // 不重签名，就可能有部分endpoint不被签名，要先写入
                for (uint8_t ix = 0; ix < writeEndpointCount; ++ix) {
                    endpoint::Endpoint ep = this->_endpoints[ix];
                    if (!(ep.flag & endpoint::flagSigned)) {
                        ret = ep.write(stream, writeBytes);
                        if (ret != ICN_RESULT_SUCCESS) {
                            return ret;
                        }
                    }
                } 
            }

            *signStartPos = stream.pos();
            // 再写入签名的endpointlist
            for (uint8_t ix = 0; ix < writeEndpointCount; ++ix) {
                endpoint::Endpoint ep = this->_endpoints[ix];
                if (isWillResign || (ep.flag & endpoint::flagSigned)) {
                    ep.flag |= endpoint::flagSigned;
                    ret = ep.write(stream, writeBytes);
                    if (ret != ICN_RESULT_SUCCESS) {
                        return ret;
                    }
                }
            }

            if (this->_snInfoList.size()) {
                uint8_t writeSnInfoCount = (uint8_t)this->_snInfoList.size();
                ret = (uint32_t)stream.writeUInt8(writeSnInfoCount, writeBytes);
                if (ret != ICN_RESULT_SUCCESS) {
                    return ret;
                }
                for (size_t ix = 0; ix < writeSnInfoCount; ++ix) {
                    ret = Info::write(stream, this->_snInfoList[ix], writeBytes);
                    if (ret != ICN_RESULT_SUCCESS) {
                        return ret;
                    }
                }
            }
            else if (this->_snList.size()) {
                uint8_t writeSnListCount = (uint8_t)this->_snList.size();
                ret = (uint32_t)stream.writeUInt8(writeSnListCount, writeBytes);
                if (ret != ICN_RESULT_SUCCESS) {
                    return ret;
                }
                for (size_t ix = 0; ix < writeSnListCount; ++ix) {
                    ret = (uint32_t)this->_snList[ix].write(stream, writeBytes);
                    if (ret != ICN_RESULT_SUCCESS) {
                        return ret;
                    }
                }
            }
            stream.writeUInt64(this->_createTime, writeBytes);
            *writeBytes = stream.pos() - startPos;
            return ICN_RESULT_SUCCESS;
        }


        uint32_t Info::readSignField(uint32_t flags, BufferStream& stream, size_t* readBytes, size_t* signStartPos) {
            uint32_t ret = ICN_RESULT_SUCCESS;
            int readSize = 0;
            uint8_t epCount = 0;
            size_t startPos = stream.pos();
            *signStartPos = 0;
            readSize = stream.readUInt8(&epCount);
            if (readSize <= 0) {
                return ICN_RESULT_PARSE_FAILED;
            }

            for (uint8_t epI = 0; epI < epCount; epI++) {
                endpoint::Endpoint endpoint;
                size_t start = stream.pos();
                ret = endpoint.read(stream);
                if (ret != ICN_RESULT_SUCCESS) {
                    return ICN_RESULT_PARSE_FAILED;
                }
                if (*signStartPos == 0 && (endpoint.flag & endpoint::flagSigned)) {
                    *signStartPos = start;
                }
                this->_endpoints.push_back(endpoint);
            }

            if (*signStartPos == 0) {
                *signStartPos = stream.pos();
            }

            if (flags & InfoBuilder::flagHasSnInfoList) {
                uint8_t snCount = 0;
                readSize = stream.readUInt8(&snCount);
                if (readSize <= 0) {
                    return ICN_RESULT_PARSE_FAILED;
                }
                for (uint8_t ix = 0; ix < snCount; ++ix) {
                    std::shared_ptr<Info> snInfo;
                    Info::read(stream, &snInfo);
                    if (!snInfo.get()) {
                        return ICN_RESULT_PARSE_FAILED;
                    }
                    this->_snInfoList.push_back(snInfo);
                }
            }
            else if (flags & InfoBuilder::flagHasSnList) {
                uint8_t snCount = 0;
                readSize = stream.readUInt8(&snCount);
                if (readSize <= 0) {
                    return ICN_RESULT_PARSE_FAILED;
                }
                for (uint8_t ix = 0; ix < snCount; ++ix) {
                    Id id;
                    int ret = id.read(stream);
                    if (ret <= 0) {
                        return ICN_RESULT_PARSE_FAILED;
                    }
                    this->_snList.push_back(id);
                }
            }

            readSize = stream.readUInt64(&this->_createTime);
            if (readSize <= 0) {
                return ICN_RESULT_PARSE_FAILED;
            }
            *readBytes = stream.pos() - startPos;

            return ICN_RESULT_SUCCESS;
        }

        uint32_t Info::verify(const uint8_t* buffer, size_t bufferBytes) const {
            int verifyResult = crypto::rsaVerifyMd5(
                this->_signature,
                sizeof(this->_signature),
                (uint8_t*)&this->_constInfo.publicKey,
                publicKeyBytes(this->_constInfo.publicKeyType),
                buffer,
                bufferBytes);
            if (verifyResult != 0) {
                return ICN_RESULT_INVALID_SIGNATURE;
            }
            return ICN_RESULT_SUCCESS;
        }

        uint32_t Info::write(BufferStream& bufferStream, const std::shared_ptr<Info>& in, size_t* pWriteBytes) {
            uint8_t flags = InfoBuilder::flagNull;
            size_t writeBytes = 0;
            size_t startPos = bufferStream.pos();
            *pWriteBytes = 0;
            int r = 0;

            if (in.get() == NULL) {
                r = bufferStream.writeUInt8(flags, &writeBytes);
                if (r != ICN_RESULT_SUCCESS) {
                    return r;
                }
                *pWriteBytes = 1;
                return ICN_RESULT_SUCCESS;
            }
            else {
                flags = 0;
            }

            if (in->_snInfoList.size()) {
                flags |= InfoBuilder::flagHasSnInfoList;
            }
            else if (in->_snList.size()) {
                flags |= InfoBuilder::flagHasSnList;
            }
            if ((in->_flags & flagSigned) || in->_secret != NULL) {
                flags |= flagSigned;
            }

            r = bufferStream.writeUInt8(flags, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            r = bufferStream.writeUInt8(in->_constInfo.areaCode.continent, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            r = bufferStream.writeUInt8(in->_constInfo.areaCode.country, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            r = bufferStream.writeUInt8(in->_constInfo.areaCode.carrier, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            r = bufferStream.writeUInt16(in->_constInfo.areaCode.city, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            r = bufferStream.writeUInt8(in->_constInfo.areaCode.inner, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            //====================================================================
            r = bufferStream.writeByteArray(in->_constInfo.deviceId, deviceIdMaxBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            r = bufferStream.writeUInt8(in->_constInfo.publicKeyType, &writeBytes);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }
            writeBytes = 0;

            switch (in->_constInfo.publicKeyType) {
            case publicKeyTypeRsa1024:
                r = bufferStream.writeByteArray(in->_constInfo.publicKey.rsa1024, publicKeyBytesRsa1024);
                if (r != ICN_RESULT_SUCCESS) {
                    return r;
                }
                writeBytes = 0;
                break;
            case publicKeyTypeRsa2048:
                r = bufferStream.writeByteArray(in->_constInfo.publicKey.rsa2048, publicKeyBytesRsa2048);
                if (r != ICN_RESULT_SUCCESS) {
                    return r;
                }
                writeBytes = 0;
                break;
            case publicKeyTypeRsa3072:
                r = bufferStream.writeByteArray(in->_constInfo.publicKey.rsa3072, publicKeyBytesRsa3072);
                if (r != ICN_RESULT_SUCCESS) {
                    return r;
                }
                writeBytes = 0;
                break;
            default:
                return ICN_RESULT_UNKNOWN_TYPE;
            }

            size_t signStartPos = 0;
            r = in->writeSignField(false, bufferStream, &writeBytes, &signStartPos);
            if (r != ICN_RESULT_SUCCESS) {
                return r;
            }

            if (flags & flags) {
                r = bufferStream.writeByteArray(in->_signature, sizeof(in->_signature));
                if (r != ICN_RESULT_SUCCESS) {
                    return r;
                }
            }
            *pWriteBytes = bufferStream.pos() - startPos;
            return ICN_RESULT_SUCCESS;
        }

        int Info::read(BufferStream& bufferStream, std::shared_ptr<Info>* out) {
            int readlen = 0;
            uint32_t ret = ICN_RESULT_PARSE_FAILED;
            size_t startPos = bufferStream.pos();
            Info* info = new Info();

            do {
                uint8_t flags = 0;
                readlen = bufferStream.readUInt8(&(flags));
                if (readlen <= 0) {
                    break;
                }

                if (flags == InfoBuilder::flagNull) {
                    ret = ICN_RESULT_SUCCESS;
                    break;
                }
                
                readlen = bufferStream.readUInt8(&(info->_constInfo.areaCode.continent));
                if (readlen <= 0) {
                    break;
                }

                readlen = bufferStream.readUInt8(&(info->_constInfo.areaCode.country));
                if (readlen <= 0) {
                    break;
                }

                readlen = bufferStream.readUInt8(&(info->_constInfo.areaCode.carrier));
                if (readlen <= 0) {
                    break;
                }

                readlen = bufferStream.readUInt16(&(info->_constInfo.areaCode.city));
                if (readlen <= 0) {
                    break;
                }

                readlen = bufferStream.readUInt8(&(info->_constInfo.areaCode.inner));
                if (readlen <= 0) {
                    break;
                }
                //----------------------------------------------------------------------------
                readlen = bufferStream.readByteArray(info->_constInfo.deviceId, deviceIdMaxBytes);
                if (readlen <= 0) {
                    break;
                }

                readlen = bufferStream.readUInt8(&(info->_constInfo.publicKeyType));
                if (readlen <= 0) {
                    break;
                }

                if (info->_constInfo.publicKeyType == publicKeyTypeRsa1024) {
                    readlen = bufferStream.readByteArray(info->_constInfo.publicKey.rsa1024, publicKeyBytesRsa1024);
                    if (readlen <= 0)
                    {
                        break;
                    }
                }
                else if (info->_constInfo.publicKeyType == publicKeyTypeRsa2048) {
                    readlen = bufferStream.readByteArray(info->_constInfo.publicKey.rsa2048, publicKeyBytesRsa2048);
                    if (readlen <= 0)
                    {
                        break;
                    }
                }
                else if (info->_constInfo.publicKeyType == publicKeyTypeRsa3072) {
                    readlen = bufferStream.readByteArray(info->_constInfo.publicKey.rsa3072, publicKeyBytesRsa3072);
                    if (readlen <= 0) {
                        break;
                    }
                }
                else {
                    ret = ICN_RESULT_UNKNOWN_TYPE;
                    break;
                }

                size_t readSize = 0;
                size_t signStartPos = 0;
                ret = info->readSignField(flags, bufferStream, &readSize, &signStartPos);
                if (ret != ICN_RESULT_SUCCESS) {
                    break;
                }
                ret = ICN_RESULT_PARSE_FAILED;
                if (flags & Info::flagSigned) {
                    assert(signStartPos > 0);
                    size_t signSize = bufferStream.pos() - signStartPos;
                    readlen = bufferStream.readByteArray(info->_signature, sizeof(info->_signature));
                    if (readlen <= 0) {
                        break;
                    }
                    ret = info->verify(bufferStream.buffer() + signStartPos, signSize);
                }
                else {
                    ret = ICN_RESULT_SUCCESS;
                }
            } while (false);

            if (ret) {
                delete info;
                *out = std::shared_ptr<Info>(NULL);
                return -1;
            }
            else {
                info->constInfo().toId(&info->_peerid);
                *out = std::shared_ptr<Info>(info);
                return (int)(bufferStream.pos() - startPos);
            }
        }


		/*InfoBuilder InfoBuilder::begin(uint32_t flags) {

		}

		InfoBuilder& InfoBuilder::setConstInfo(const Const& constInfo) {

		}

		InfoBuilder& InfoBuilder::addEndpoint(const endpoint::Endpoint& endpoint) {

		}

		InfoBuilder& InfoBuilder::clearEndpoint() {

		}

		InfoBuilder& InfoBuilder::addSn(const Id& id) {

		}

		InfoBuilder& InfoBuilder::addSnInfo(std::shared_ptr<Info> peerInfo) {

		}

		InfoBuilder& InfoBuilder::setSecret(const Secret& secret) {

		}

		std::shared_ptr<Info> InfoBuilder::finish(const Secret* signSecret) {

		}*/
	}
}