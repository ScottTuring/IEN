#pragma once

int32_t IcnAtomicCompareAndSwap32(volatile int32_t* ptr
	, int32_t oldValue
	, int32_t newValue) {

	__atomic_compare_exchange_n(ptr, &oldValue, newValue
		, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED);

	return oldValue;
}


int32_t IcnAtomicInc32(volatile int32_t* ptr) {
	return IcnAtomicAdd32(ptr, 1);
}

int32_t IcnAtomicAdd32(volatile int32_t* ptr, int32_t value) {
	return value + __atomic_fetch_add(ptr, value, __ATOMIC_SEQ_CST);
}

int32_t IcnAtomicDec32(volatile int32_t* ptr) {
	return __atomic_fetch_sub(ptr, 1, __ATOMIC_SEQ_CST) - 1;
}

int32_t IcnAtomicExchange32(volatile int32_t* ptr, int32_t value) {
	return __atomic_exchange_n(ptr, value, __ATOMIC_SEQ_CST);
}

int32_t IcnAtomicAnd32(volatile int32_t* ptr, int32_t value) {
	return __atomic_fetch_and(ptr, value, __ATOMIC_SEQ_CST);
}

int32_t IcnAtomicOr32(volatile int32_t* ptr, int32_t value) {
	return __atomic_fetch_or(ptr, value, __ATOMIC_SEQ_CST);
}

int32_t IcnAtomicXor32(volatile int32_t* ptr, int32_t value) {
	return __atomic_fetch_xor(ptr, value, __ATOMIC_SEQ_CST);
}

#if defined(ICN_ARCH_CPU_64_BITS)
int64_t IcnAtomicCompareAndSwap64(volatile int64_t* ptr
	, int64_t oldValue
	, int64_t newValue) {

	__atomic_compare_exchange_n(ptr, &oldValue, newValue
		, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED);

	return oldValue;
}

int64_t IcnAtomicExchange64(volatile int64_t* ptr, int64_t value) {
	return __atomic_exchange_n(ptr, value, __ATOMIC_SEQ_CST);
}

int64_t IcnAtomicAnd64(volatile int64_t* ptr, int64_t value) {
	return __atomic_fetch_and(ptr, value, __ATOMIC_SEQ_CST);
}

int64_t IcnAtomicOr64(volatile int64_t* ptr, int64_t value) {
	return __atomic_fetch_or(ptr, value, __ATOMIC_SEQ_CST);
}

int64_t IcnAtomicXor64(volatile int64_t* ptr, int64_t value) {
	return __atomic_fetch_xor(ptr, value, __ATOMIC_SEQ_CST);
}

#endif // ICN_ARCH_CPU_64_BITS

