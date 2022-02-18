/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include "./basic_type.h"

// 获取一个[0, UINT64_MAX]区间的随机数
uint64_t random64();

// 获取一个[minValue, maxValue]区间的随机数
int32_t randomRange32(int32_t minValue, int32_t maxValue);

// 获取一个[0, maxValue)区间的随机数
uint64_t randomRange64(uint64_t maxValue);


void randomBytes(uint8_t* lpBuffer, size_t size);