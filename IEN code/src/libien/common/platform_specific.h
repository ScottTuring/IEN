﻿
#pragma once

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif  // __APPLE__


// 用以标识当前平台
#if defined(_WIN32)
	#define ICN_OS_WIN	1
#elif (defined(ANDROID) || defined(__ANDROID__))
	#define ICN_OS_ANDROID 1
#elif defined(__APPLE__)
	#define ICN_OS_MACOSX 1
	#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
		#define ICN_OS_IOS 1
	#endif  // defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#elif defined(__linux__)
	#define ICN_OS_LINUX 1
	#if defined(__GLIBC__) && !defined(__UCLIBC__)
		#define LIBC_GLIBC
	#endif // glibc
#elif defined(__FreeBSD__)
	#define ICN_OS_FREEBSD 1
#elif defined(__OpenBSD__)
	#define ICN_OS_OPENBSD 1
#else
	#error "unsupport platfrom!!!!"
#endif // platfrom

// 用以区分BSD系统
#if defined(ICN_OS_FREEBSD) || defined(ICN_OS_OPENBSD)
	#define ICN_OS_BSD 1
#endif // BSD

// 用以区分是否是posix标准系统
#if defined(ICN_OS_MACOSX) || defined(ICN_OS_LINUX) || defined(ICN_OS_FREEBSD) || \
	defined(ICN_OS_OPENBSD) || defined(OS_SOLARIS) || defined(ICN_OS_ANDROID)
	#define ICN_OS_POSIX		1
#endif // POSIX

// 用以区分当前使用的编译器，目前有GCC和MSVC
#if defined(__GNUC__)
	#define ICN_COMPILER_GCC	1
    #if defined(__clang__)
        #define ICN_COMPILER_CLANG  1
    #endif // __clang__
#elif defined(_MSC_VER)
	#define ICN_COMPILER_MSVC	1
#else
	#error "unsupport compiler!!!"
#endif // complier

// 用以区分当前的CPU架构
#if defined(_M_IX86) || defined(__i386__) || defined(i386)
	#define ICN_ARCH_CPU_X86_FAMILY		1
	#define ICN_ARCH_CPU_X86			1
	#define ICN_ARCH_CPU_32_BITS		1
	#define ICN_ARCH_CPU_LITTLE_ENDIAN	1
#elif defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
	#define ICN_ARCH_CPU_X86_FAMILY		1
	#define ICN_ARCH_CPU_X86_64			1
	#define ICN_ARCH_CPU_64_BITS		1
	#define ICN_ARCH_CPU_LITTLE_ENDIAN	1
#elif defined(__ARMEL__)
	#define ICN_ARCH_CPU_ARM_FAMILY		1
	#define ICN_ARCH_CPU_ARMEL			1
	#if defined(__aarch64__)
		#define ICN_ARCH_CPU_64_BITS	1
	#else // arch32
		#define ICN_ARCH_CPU_32_BITS	1	
	#endif // arch64
	#define ICN_ARCH_CPU_LITTLE_ENDIAN	1
#elif defined(__AARCH64EL__)
	#define ICN_ARCH_CPU_ARM_FAMILY		1
	#define ICN_ARCH_CPU_ARMEL			1
	#define ICN_ARCH_CPU_64_BITS		1
	#define ICN_ARCH_CPU_LITTLE_ENDIAN	1
#elif defined(__MIPSEL__)
	#define ICN_ARCH_CPU_MIPS_FAMILY	1
	#define ICN_ARCH_CPU_MIPSEL			1
	#define ICN_ARCH_CPU_32_BITS		1
	#define ICN_ARCH_CPU_LITTLE_ENDIAN	1
#else
	#error "unsupport architecture!!!!"
#endif // architecture

// 不同平台下wchar_t类型的检测，用以定义不同的字符串类型
#if defined(ICN_OS_WIN)
	#define ICN_WCHAR_T_UTF16			1
    #define ICN_WCHAR_T_UNSIGNED        1
#elif defined(ICN_OS_POSIX) && defined(ICN_COMPILER_GCC) && defined(__WCHAR_MAX__)
	#if (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
		#define ICN_WCHAR_T_UTF32		1
	#elif  (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
		#define ICN_WCHAR_T_UTF16		1
	#elif (__WCHAR_MAX__ == 0x7f || __WCHAR_MAX__ == 0xff)
		// 安卓2.3以前wchar_t是一个字节
		#define ICN_WCHAR_T_UTF8		1
	#else
		#error "unsupport posix wchar_t type!"
	#endif // __WCHAR_MAX__

    #if (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0x7f)
		#define ICN_WCHAR_T_SIGNED	    1
	#elif  (__WCHAR_MAX__ == 0xffffffff || __WCHAR_MAX__ == 0xffff || __WCHAR_MAX__ == 0xff)
		#define ICN_WCHAR_T_UNSIGNED	1
	#else
		#error "unsupport posix wchar_t type!"
	#endif // __WCHAR_MAX__
#else // other system
	#error "unsupport system wchar_t type!"
#endif // ICN_OS_WIN


// sockaddr_in是否有sin_len字段
#if defined(ICN_OS_MACOSX)
    #define ICN_HAVE_SIN_LEN    1
#endif // ICN_OS_MACOSX


// sockaddr_in6是否有sin6_len字段
#if defined(ICN_OS_MACOSX)
    #define ICN_HAVE_SIN6_LEN   1
#endif // ICN_OS_MACOSX

