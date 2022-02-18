/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <vector>
#include <memory>
#include "../common/common.h"
#include "./endpoint.h"
#include "./crypto.h"

namespace ien {
	namespace peer {
		struct Area {
			uint8_t continent;
			uint8_t country;
			uint8_t carrier;
			uint16_t city;
			uint8_t inner;
		};

		const static long idBytes = 32;
		const static long idChars = 2 * idBytes;

		struct Id {
			uint8_t pid[idBytes];

			static const Id& defaultId() {
				const static Id defaultId = Id {
					{0}
				};
				return defaultId;
			}

			int compare(const Id* right) const;
			void toString(char out[idChars]) const;

			bool operator< (const Id& right) const {
				return this->compare(&right) < 0;
			}

			int read(BufferStream& bufferStream) {
				return bufferStream.readByteArray(this->pid, idBytes);
			}

			int write(BufferStream& bufferStream, size_t* pWriteBytes) const {
				*pWriteBytes = idBytes;
				return bufferStream.writeByteArray(this->pid, idBytes);
			}
		};


		const static uint8_t publicKeyTypeRsa1024 = 0;
		const static uint32_t publicKeyBytesRsa1024 = 162;
		const static uint32_t secretKeyMaxBytesRsa1024 = 610;
		
		const static uint8_t publicKeyTypeRsa2048 = 1;
		const static uint32_t publicKeyBytesRsa2048 = 294;
		const static uint32_t secretKeyMaxBytesRsa2048 = 1194;

		const static uint8_t publicKeyTypeRsa3072 = 2;
		const static uint32_t publicKeyBytesRsa3072 = 422;
		const static uint32_t secretKeyMaxBytesRsa3072 = 1770;

		uint32_t rsaBits(uint8_t keyType);
		uint32_t publicKeyBytes(uint8_t keyType);
		uint32_t secretKeyMaxBytes(uint8_t keyType);
		
		const static long deviceIdMaxBytes = 128;

		struct Secret {
			union {
				uint8_t rsa1024[secretKeyMaxBytesRsa1024];
				uint8_t rsa2048[secretKeyMaxBytesRsa2048];
				uint8_t rsa3072[secretKeyMaxBytesRsa3072];
			} secret;
			uint16_t secretLength;
		};

		struct Const {
			Area areaCode;
			uint8_t deviceId[deviceIdMaxBytes];
			uint8_t publicKeyType;
			union {
				uint8_t rsa1024[publicKeyBytesRsa1024];
				uint8_t rsa2048[publicKeyBytesRsa2048];
				uint8_t rsa3072[publicKeyBytesRsa3072];
			} publicKey;

			uint32_t init(
				const Area* areaCode,
				const char* deviceId,
				uint8_t publicKeyType,
				Id* outPeerid,
				Secret* outSecret
			);
			uint32_t toId(Id* outPeerid) const;
			uint32_t verify(const Id* peerid) const;
		};

		class InfoBuilder;

		class Info {
			friend class InfoBuilder;
		public:
			Info(Info* proto);
			~Info();
			inline const std::vector<endpoint::Endpoint>& endpointList() const {
				return this->_endpoints;
			}

			bool hasEndpoint(const endpoint::Endpoint& ep) const {
				for (size_t ix = 0; ix < this->_endpoints.size(); ++ix) {
					if (!this->_endpoints.at(ix).compare(&ep, false)) {
						return true;
					}
				}
				return false;
			}

			inline const Const& constInfo() const {
				return this->_constInfo;
			}

			inline const Id& id() const {
				return this->_peerid;
			}

			inline const std::vector<Id>& snList() const {
				return this->_snList;
			}

			inline const std::vector<std::shared_ptr<Info>>& snInfoList() const {
				return this->_snInfoList;
			}

			inline const Secret* secret() const {
				return this->_secret;
			}

			inline uint64_t createTime() const {
				return this->_createTime;
			}


			bool isSigned() const;
			uint32_t verify(const uint8_t* buffer, size_t bufferBytes) const;
			static int read(BufferStream& bufferStream, std::shared_ptr<Info>* out);
			static uint32_t write(BufferStream& bufferStream, const std::shared_ptr<Info>& in, size_t* pWriteBytes);
		private:
			Info();
			uint32_t readSignField(uint32_t flags, BufferStream& stream, size_t* readBytes, size_t* signStartPos);
			uint32_t writeSignField(
				bool isWillResign,
				BufferStream& stream,
				size_t* writeBytes,
				size_t* signStartPos);
		private:
			uint32_t _flags;
			Id _peerid;
			Const _constInfo;
			std::vector<endpoint::Endpoint> _endpoints;
			std::vector<Id> _snList;
			std::vector<std::shared_ptr<Info>> _snInfoList; 
			Secret* _secret;
			uint8_t _signature[crypto::signatureBytes];
			uint64_t _createTime;

		private:
			const static long flagSigned = (1 << 4);
		};



		class InfoBuilder {
		public:
			const static long flagNull = (1 << 0);
			const static long flagHasSecret = (1 << 1);
			const static long flagHasSnList = (1 << 2);
			const static long flagHasSnInfoList = (1 << 3);

			static InfoBuilder begin(uint32_t flags);
			InfoBuilder& setConstInfo(const Const& constInfo);
			InfoBuilder& addEndpoint(const endpoint::Endpoint& endpoint);
			InfoBuilder& clearEndpoint();
			InfoBuilder& addSn(const Id& id);
			InfoBuilder& addSnInfo(std::shared_ptr<Info> peerInfo);
			InfoBuilder& setSecret(const Secret& secret);
			std::shared_ptr<Info> finish(const Secret* signSecret);
		private:
			Info* result;
		};

	}
}


