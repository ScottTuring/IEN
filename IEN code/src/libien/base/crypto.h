/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
Intelligent_Eco_Networking_IEN_an_Advanced_Future_Internet_of_intelligence_for_Digital_Social_Economic_Ecosystem
**************************************************/
#pragma once
#include "../common/common.h"

namespace ien {
	namespace crypto {
		const static long aesBytes = 32;
		const static long mixHashBytes = 8;
		const static long signatureTinyBytes = 16;
		const static long signatureBytes = 128;
		const static long signatureMaxBytes = 1024;

		int aesEncryptDataWithPadding(
			const uint8_t aesKey[aesBytes],
			uint8_t* input,
			size_t dataLen,
			size_t iLen,
			uint8_t* output,
			size_t oLen);

		int aesDecryptDataWithPadding(
			const uint8_t aesKey[aesBytes],
			const uint8_t* input,
			size_t iLen,
			uint8_t* output,
			size_t oLen);

		void hashAesKey(
			const uint8_t aesKey[aesBytes],
			uint8_t keyHash[mixHashBytes]);

		bool verifyAesKeyHash(
			const uint8_t aesKey[aesBytes],
			const uint8_t keyHash[mixHashBytes]);

		int aesGenerateKey(uint8_t aesKey[aesBytes]);

		int sha256Hash(const uint8_t* data, size_t len, uint8_t hash[32]);

		int rsaEncrypt(const uint8_t* data, size_t len, const uint8_t* publicKey, size_t publicKeyLen, uint8_t* output, size_t* olen, size_t osize);
		
		int rsaDecrypt(const uint8_t* data, const uint8_t* secret, size_t secretLen, uint8_t* output, size_t* outputLen, size_t osize, size_t* outKeyBytes);
		
		int rsaSignMd5(const uint8_t* data, size_t len, const uint8_t* secret, size_t secretLen, uint8_t* output, size_t* outputLen);
		int rsaVerifyMd5(const uint8_t* sign, size_t signLen, const uint8_t* publicKey, size_t publicLen, const uint8_t* data, size_t dataLen);

		int rsaGenerate(uint32_t bits, uint8_t* publicKey, size_t* inoutPublicLen, uint8_t* secret, size_t* inoutSecretLen);
	}
}

