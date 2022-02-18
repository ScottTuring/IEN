
/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
/*
ret = *ptr;
if (*ptr == oldValue) {
	*ptr = newValue;
}
return ret;
*/
int32_t IcnAtomicCompareAndSwap32(volatile int32_t* ptr
	, int32_t oldValue
	, int32_t newValue);

/*
ret = *ptr;
*ptr = value;
return ret;
*/
int32_t IcnAtomicExchange32(volatile int32_t* ptr, int32_t value);

/*
return *ptr += value;
*/
int32_t IcnAtomicAdd32(volatile int32_t* ptr, int32_t value);

/*
return *ptr += 1;
*/
int32_t IcnAtomicInc32(volatile int32_t* ptr);


/*
return *ptr -= 1;
*/
int32_t IcnAtomicDec32(volatile int32_t* ptr);

/*
tmp = *ptr;
*prt op= value;
return tmp;
*/
int32_t IcnAtomicAnd32(volatile int32_t* ptr, int32_t vlaue);
int32_t IcnAtomicOr32(volatile int32_t* ptr, int32_t vlaue);
int32_t IcnAtomicXor32(volatile int32_t* ptr, int32_t vlaue);


#if defined(ICN_ARCH_CPU_64_BITS)
int64_t IcnAtomicCompareAndSwap64(volatile int64_t* ptr
	, int64_t oldValue
	, int64_t newValue);
int64_t IcnAtomicExchange64(volatile int64_t* ptr, int64_t value);

int64_t IcnAtomicAnd64(volatile int64_t* ptr, int64_t vlaue);
int64_t IcnAtomicOr64(volatile int64_t* ptr, int64_t vlaue);
int64_t IcnAtomicXor64(volatile int64_t* ptr, int64_t vlaue);

#endif // ICN_ARCH_CPU_64_BITS

typedef void* IcnAtomicPointerType;
IcnAtomicPointerType IcnAtomicCompareAndSwapPointer(volatile IcnAtomicPointerType* ptr
	, IcnAtomicPointerType oldValue
	, IcnAtomicPointerType newValue);
IcnAtomicPointerType IcnAtomicExchangePointer(volatile IcnAtomicPointerType* ptr
    , IcnAtomicPointerType value);