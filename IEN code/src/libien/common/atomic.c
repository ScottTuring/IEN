#include "./common.h"

#if defined(ICN_OS_WIN)
#include "./atomic_msvc.h"
#else
#include "./atomic_gcc.h"
#endif

#if defined(ICN_ARCH_CPU_32_BITS)

IcnAtomicPointerType IcnAtomicCompareAndSwapPointer(volatile IcnAtomicPointerType* ptr
	, IcnAtomicPointerType oldValue
	, IcnAtomicPointerType newValue) {
	return (IcnAtomicPointerType)IcnAtomicCompareAndSwap32((volatile int32_t*)ptr,
		(int32_t)oldValue,
		(int32_t)newValue);
}

IcnAtomicPointerType IcnAtomicExchangePointer(volatile IcnAtomicPointerType* ptr, IcnAtomicPointerType value) {
	return (IcnAtomicPointerType)IcnAtomicExchange32((volatile int32_t*)ptr,
		(int32_t)value);
}

#else // ICN_ARCH_CPU_64_BITS 

IcnAtomicPointerType IcnAtomicCompareAndSwapPointer(volatile IcnAtomicPointerType* ptr
	, IcnAtomicPointerType oldValue
	, IcnAtomicPointerType newValue) {
	return (IcnAtomicPointerType)IcnAtomicCompareAndSwap64((volatile int64_t*)ptr,
		(int64_t)oldValue,
		(int64_t)newValue);
}

IcnAtomicPointerType IcnAtomicExchangePointer(volatile IcnAtomicPointerType* ptr, IcnAtomicPointerType value) {
	return (IcnAtomicPointerType)IcnAtomicExchange64((volatile int64_t*)ptr,
		(int64_t)value);
}

#endif // ICN_ARCH_CPU_64_BITS