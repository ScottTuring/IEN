/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once

#include <list>
#include <map>
#include <memory>

#include "../base/base.h"
#include "../common/common.h"

#define MIX_HASH_4_KEY_LIMIT 4 // 每个KEY最多计算的HASH数

namespace ien {

    namespace history {

        class KeyManager {
        public:
            KeyManager();
            ~KeyManager();

            uint32_t addKey(const uint8_t aesKey[crypto::aesBytes],
                            const peer::Id& peerid,
                            // confirm the key by remote
                            bool confirmedByRemote);

            // should always sync finished
            uint32_t findByMixHash(const uint8_t mixHash[crypto::mixHashBytes],
                                   uint8_t outKey[crypto::aesBytes],
                                   peer::Id* outPeerid,
                                   // if touch stub in manager to extend key's expire time
                                   bool touch,
                                   // confirm the key by remote
                                   bool confirmedByRemote);

            // should always sync finished
            uint32_t getKeyByRemote(const peer::Id& peerid,
                                    uint8_t outKey[crypto::aesBytes],
                                    // in: if create a new key for remote when no key exists 
                                    // out: if set to true when input, output value of this param shows if the key returned is newly created
                                    bool* inoutNew,
                                    // out: set 'true' when the key is confirmed by remote
                                    bool* outIsConfirmByRemote,
                                    // if touch stub in manager to extend key's expire time
                                    bool touch);

            static bool calcKeyHash(const uint8_t aesKey[crypto::aesBytes],
                                    uint8_t hash[crypto::mixHashBytes]);
        };

    }

}
