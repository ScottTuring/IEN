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
#include "../base/base.h"
#include "./package.h"

namespace ien {
    namespace protocol {

		const static size_t maxMergeLength = 32;

		int decodePackages(
			const uint8_t* buffer,
			size_t length,
			const Package* refPackage,
			Package** result,
			bool hasExchange);

		int encodePackages(
			const Package** packages,
			size_t packageCount,
			const Package* refPackage,
			uint8_t* buffer,
			size_t bufferLength,
			size_t* pWriteBytes);
    }
}