
#include <mbedtls/sha256.h>
#include <mbedtls/pk.h>
#include <mbedtls/pk_internal.h>
#include <mbedtls/md5.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/aes.h>
#include "./crypto.h"

namespace ien {
    namespace crypto {

        int aesEncryptDataWithPadding(const uint8_t aesKey[aesBytes], uint8_t* input, size_t dataLen, size_t iLen, uint8_t* output, size_t oLen)
        {
            mbedtls_aes_context ctx;
            mbedtls_aes_init(&ctx);
            mbedtls_aes_setkey_enc(&ctx, aesKey, aesBytes * 8);

            size_t targetLen = (dataLen / 16 + 1) * 16; // padding后的数据长度
            if (oLen < targetLen || iLen < targetLen)
            {
                return -1;
            }
            // 得到应该padding的长度
            uint8_t padding = (uint8_t)(targetLen - dataLen);

            // 在空余长度处填充padding
            for (uint8_t i = 0; i < padding; i++)
            {
                input[dataLen + i] = padding;
            }

            uint8_t iv[16] = { 0 };
            int ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, targetLen, iv, input, output);
            mbedtls_aes_free(&ctx);
            if (ret != 0)
            {
                return ret;
            }

            return (int)targetLen;
        }

        int aesDecryptDataWithPadding(const uint8_t aesKey[aesBytes], const uint8_t* input, size_t iLen, uint8_t* output, size_t oLen)
        {
            if (oLen < iLen)
            {
                return -1;
            }
            mbedtls_aes_context ctx;
            mbedtls_aes_init(&ctx);
            mbedtls_aes_setkey_dec(&ctx, aesKey, aesBytes * 8);
            uint8_t iv[16] = { 0 };
            int ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, iLen, iv, input, output);
            mbedtls_aes_free(&ctx);
            if (ret != 0)
            {
                return ret;
            }
            uint8_t padding = output[iLen - 1];
            for (size_t i = 0; i < padding; i++)
            {
                if (output[iLen - 1 - i] != padding)
                {
                    return -1;
                }
            }
            return (int)(iLen - padding);
        }

        void hashAesKey(const uint8_t aesKey[aesBytes], uint8_t keyHash[mixHashBytes]) {
            uint8_t sha256Result[32];
            sha256Hash(aesKey, aesBytes, sha256Result);
            memcpy(keyHash, sha256Result, 8);
            return;
        }


        bool verifyAesKeyHash(const uint8_t aesKey[aesBytes], const uint8_t keyHash[mixHashBytes]) {
            uint8_t sha256Result[32];
            sha256Hash(aesKey, aesBytes, sha256Result);
            if (memcmp(sha256Result, keyHash, 8) == 0) {
                return true;
            }
            return false;
        }

        int sha256Hash(const uint8_t* data, size_t len, uint8_t hash[32]) {
            return mbedtls_sha256_ret(data, len, hash, 0);;
        }


        int aesGenerateKey(uint8_t aesKey[aesBytes]) {
            randomBytes(aesKey, aesBytes);
            return ICN_RESULT_SUCCESS;
        }

        int rsaEncrypt(const uint8_t* data, size_t len, const uint8_t* publicKey, size_t publicKeyLen, uint8_t* output, size_t* olen, size_t osize)
        {
            mbedtls_pk_context pk;
            mbedtls_pk_init(&pk);
            int ret = mbedtls_pk_parse_public_key(&pk, publicKey, publicKeyLen);
            if (ret)
            {
                return ret;
            }

            mbedtls_entropy_context entropy;
            mbedtls_entropy_init(&entropy);

            mbedtls_ctr_drbg_context ctr_drbg;
            mbedtls_ctr_drbg_init(&ctr_drbg);

            char* personalization = "ien";

            mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                (const unsigned char*)personalization,
                strlen(personalization));

            ret = mbedtls_pk_encrypt(&pk, data, len, output, olen, osize, mbedtls_ctr_drbg_random, &ctr_drbg);
            mbedtls_pk_free(&pk);
            mbedtls_entropy_free(&entropy);
            mbedtls_ctr_drbg_free(&ctr_drbg);
            return ret;
        }

        int rsaDecrypt(const uint8_t* data, const uint8_t* secret, size_t secretLen, uint8_t* output, size_t* outputLen, size_t osize, size_t* outKeyBytes)
        {
            mbedtls_entropy_context entropy;
            mbedtls_entropy_init(&entropy);

            mbedtls_ctr_drbg_context ctr_drbg;
            mbedtls_ctr_drbg_init(&ctr_drbg);

            char* personalization = "ien";

            mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                (const unsigned char*)personalization,
                strlen(personalization));

            mbedtls_pk_context spk;
            mbedtls_pk_init(&spk);
            int ret = mbedtls_pk_parse_key(&spk, secret, secretLen, NULL, 0);
            if (ret)
            {
                return ret;
            }

            if (outKeyBytes)
            {
                *outKeyBytes = mbedtls_pk_rsa(spk)->len;
            }

            ret = mbedtls_pk_decrypt(&spk, data, mbedtls_pk_rsa(spk)->len, output, outputLen, osize, mbedtls_ctr_drbg_random, &ctr_drbg);
            mbedtls_pk_free(&spk);
            mbedtls_entropy_free(&entropy);
            mbedtls_ctr_drbg_free(&ctr_drbg);
            return ret;
        }

        int rsaSignMd5(const uint8_t* data, size_t len, const uint8_t* secret, size_t secretLen, uint8_t* output, size_t* outputLen)
        {
            mbedtls_entropy_context entropy;
            mbedtls_entropy_init(&entropy);

            mbedtls_ctr_drbg_context ctr_drbg;
            mbedtls_ctr_drbg_init(&ctr_drbg);

            char* personalization = "ien";

            mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                (const unsigned char*)personalization,
                strlen(personalization));

            uint8_t md[16];
            int ret = mbedtls_md5_ret(data, len, md);
            if (ret)
            {
                return ret;
            }

            mbedtls_pk_context spk;
            mbedtls_pk_init(&spk);
            ret = mbedtls_pk_parse_key(&spk, secret, secretLen, NULL, 0);
            if (ret)
            {
                return ret;
            }

            ret = mbedtls_pk_sign(&spk, MBEDTLS_MD_MD5, md, 16, output, outputLen, mbedtls_ctr_drbg_random, &ctr_drbg);
            mbedtls_pk_free(&spk);
            mbedtls_entropy_free(&entropy);
            mbedtls_ctr_drbg_free(&ctr_drbg);
            return ret;
        }

        int rsaVerifyMd5(const uint8_t* sign, size_t signLen, const uint8_t* publicKey, size_t publicLen, const uint8_t* data, size_t dataLen)
        {
            uint8_t md[16];
            int ret = mbedtls_md5_ret(data, dataLen, md);
            if (ret)
            {
                return ret;
            }

            mbedtls_pk_context pk;
            mbedtls_pk_init(&pk);

            ret = mbedtls_pk_parse_public_key(&pk, publicKey, publicLen);
            if (ret)
            {
                return ret;
            }

            ret = mbedtls_pk_verify(&pk, MBEDTLS_MD_MD5, md, sizeof(md), sign, signLen);
            mbedtls_pk_free(&pk);
            return ret;
        }

        int rsaGenerate(uint32_t bits, uint8_t* publicKey, size_t* inoutPublicLen, uint8_t* secret, size_t* inoutSecretLen)
        {
            mbedtls_pk_context pk;
            mbedtls_entropy_context entropy;
            mbedtls_ctr_drbg_context ctr_drbg;

            mbedtls_ctr_drbg_init(&ctr_drbg);
            mbedtls_entropy_init(&entropy);
            mbedtls_pk_init(&pk);
            mbedtls_pk_setup(&pk, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));

            char* personalization = "ien";
            mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                (const unsigned char*)personalization,
                strlen(personalization));

            int ret = 0;
            do {
                mbedtls_rsa_context* ctx = mbedtls_pk_rsa(pk);
                // mbedtls_rsa_init(ctx, MBEDTLS_RSA_PKCS_V15, 0);
                ret = mbedtls_rsa_gen_key(ctx, mbedtls_ctr_drbg_random, &ctr_drbg, bits, 65537);
                if (ret)
                {
                    break;
                }
                int secretLen = mbedtls_pk_write_key_der(&pk, secret, *inoutSecretLen);
                if (secretLen < 0) {
                    ret = secretLen;
                    break;
                }

                if (*inoutSecretLen != secretLen)
                {
                    // shift
                    memmove(secret, secret + (*inoutSecretLen - secretLen), secretLen);
                }
                *inoutSecretLen = secretLen;

                int publicLen = mbedtls_pk_write_pubkey_der(&pk, publicKey, *inoutPublicLen);
                if (publicLen < 0) {
                    ret = publicLen;
                    break;
                }
                if (*inoutPublicLen != publicLen)
                {
                    // shift
                    memmove(publicKey, publicKey + (*inoutPublicLen - publicLen), publicLen);
                }
                *inoutPublicLen = publicLen;

            } while (false);

            mbedtls_pk_free(&pk);
            mbedtls_entropy_free(&entropy);
            mbedtls_ctr_drbg_free(&ctr_drbg);

            return ret;
        }
    }
}
