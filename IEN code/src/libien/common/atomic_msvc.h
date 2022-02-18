#pragma once

int32_t IcnAtomicCompareAndSwap32(volatile int32_t* ptr
	, int32_t oldValue
	, int32_t newValue) {

	LONG result = InterlockedCompareExchange(
		(volatile LONG*)ptr,
		(LONG)newValue,
		(LONG)oldValue);

	return (int32_t)(result);
}

int32_t IcnAtomicInc32(volatile int32_t* ptr) {
	LONG ret = InterlockedIncrement((volatile LONG*)ptr);

	return (int32_t)ret;
}

int32_t IcnAtomicAdd32(volatile int32_t* ptr, int32_t value) {
	LONG ret = value + InterlockedExchangeAdd((volatile LONG*)ptr, (LONG)value);

	return (int32_t)ret;
}

int32_t IcnAtomicDec32(volatile int32_t* ptr) {
	LONG ret = InterlockedDecrement((volatile LONG*)ptr);

	return (int32_t)ret;
}


int32_t IcnAtomicExchange32(volatile int32_t* ptr, int32_t value) {
	LONG result = InterlockedExchange(
		(volatile LONG*)ptr,
		(LONG)value);

	return (int32_t)result;
}

int32_t IcnAtomicAnd32(volatile int32_t* ptr, int32_t value) {
	return (int32_t)InterlockedAnd((volatile LONG*)ptr, (LONG)value);
}

int32_t IcnAtomicOr32(volatile int32_t* ptr, int32_t value) {
	return (int32_t)InterlockedOr((volatile LONG*)ptr, (LONG)value);
}

int32_t IcnAtomicXor32(volatile int32_t* ptr, int32_t value) {
	return (int32_t)InterlockedXor((volatile LONG*)ptr, (LONG)value);
}

#if defined(ICN_ARCH_CPU_64_BITS)
int64_t IcnAtomicCompareAndSwap64(volatile int64_t* ptr
	, int64_t oldValue
	, int64_t newValue) {

	LONG64 result = InterlockedCompareExchange64(
		(volatile LONG64*)ptr,
		(LONG64)newValue,
		(LONG64)oldValue);

	return (int64_t)(result);
}

int64_t IcnAtomicExchange64(volatile int64_t* ptr, int64_t value) {
	LONG64 result = InterlockedExchange64(
		(volatile LONG64*)ptr,
		(LONG64)value);

	return (int64_t)result;
}

int64_t IcnAtomicAnd64(volatile int64_t* ptr, int64_t value) {
	return (int64_t)InterlockedAnd64((volatile LONG64*)ptr, (LONG64)value);
}

int64_t IcnAtomicOr64(volatile int64_t* ptr, int64_t value) {
	return (int64_t)InterlockedOr64((volatile LONG64*)ptr, (LONG64)value);
}
int64_t IcnAtomicXor64(volatile int64_t* ptr, int64_t value) {
	return (int64_t)InterlockedXor64((volatile LONG64*)ptr, (LONG64)value);
}
#endif // ICN_ARCH_CPU_64_BITS