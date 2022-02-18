#pragma once

#include "./platform_specific.h"
#include "./compiler_specific.h"
#include "./global_config.h"


// C99��ͳһ��ƽ̨���ݶ����ļ���������������Ҫ�и�ͷ�ļ���֧��
#if !defined(__STDC_CONSTANT_MACROS)
#define __STDC_CONSTANT_MACROS
#endif // !__STDC_CONSTANT_MACROS

#if !defined(__STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS
#endif // !__STDC_LIMIT_MACROS

#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif // !__STDC_FORMAT_MACROS

#if defined(__cplusplus)
#include <cstddef>
#else // C
#include <stddef.h>
#endif // C

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#if defined(__OBJC__)
#include <objc/objc.h>
#endif //objc

// size_t���ͳ�����
#if defined(ICN_ARCH_CPU_32_BITS)
#define SIZE_T_C(val) UINT32_C(val)
#elif defined(ICN_ARCH_CPU_64_BITS)
#define SIZE_T_C(val) UINT64_C(val)
#else // unknown arch
#error("CPU arch not support!!")
#endif // arch

// ƽ̨����ֽڳ������͵�prinf��scanf֧�֣�����size_t��ptrdiff_t
#if defined(ICN_ARCH_CPU_32_BITS)

#define PRIdVT PRId32
#define PRIiVT PRIi32
#define PRIoVT PRIo32
#define PRIuVT PRIu32
#define PRIxVT PRIx32
#define PRIXVT PRIX32

#define SCNdVT SCNd32
#define SCNiVT SCNi32
#define SCNoVT SCNo32
#define SCNuVT SCNu32
#define SCNxVT SCNx32

#else // ICN_ARCH_CPU_64_BITS

#define PRIdVT PRId64
#define PRIiVT PRIi64
#define PRIoVT PRIo64
#define PRIuVT PRIu64
#define PRIxVT PRIx64
#define PRIXVT PRIX64

#define SCNdVT SCNd64
#define SCNiVT SCNi64
#define SCNoVT SCNo64
#define SCNuVT SCNu64
#define SCNxVT SCNx64

#endif // ICN_ARCH_CPU_64_BITS


#if !defined(ICN_OS_WIN)
// �ӿڲ��BOOL���Ͷ���
#if !defined(BOOL)

#if !defined(__OBJC__)
typedef int BOOL;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#else // __OBJC__

#ifndef FALSE
#define FALSE               NO
#endif

#ifndef TRUE
#define TRUE                YES
#endif
#endif // __OBJC__

#endif	// !BOOL
#endif // !ICN_OS_WIN

// C++������BOOL��bool������ת��
#if defined(__cplusplus)
#define BOOLTobool(bv) (!!(bv))
#define boolToBOOL(bv) ((bv)? TRUE : FALSE)
#endif // __cplusplus

// �ַ����͵Ķ���
#if defined(ICN_COMPILER_CLANG) && defined(__cplusplus)
typedef char        char8_t;

// clang����������������char16_t��char32_t�Ķ���
#ifndef _LIBCPP_VERSION
typedef uint16_t	char16_t;
typedef uint32_t	char32_t;
#endif // !_LIBCPP_VERSION

#elif defined(ICN_COMPILER_GCC) && defined(__GXX_EXPERIMENTAL_CXX0X__)
    // gcc ��ָ����c++0x��׼�Ժ󣬻�����charX_t����
typedef char		char8_t;
#elif defined(ICN_COMPILER_MSVC) && (_MSC_VER >= 1600) && defined(__cplusplus)
    // vs2010���Ժ�汾֧��c++0x
typedef char		char8_t;
#else

#if defined(ICN_COMPILER_MSVC)
#define _CHAR16T // VS2010ͷ�ļ�yval.h���涨����char16_t��char32_t
#endif // ICN_COMPILER_MSVC

typedef char		char8_t;
typedef uint16_t	char16_t;
typedef uint32_t	char32_t;

#endif // ICN_COMPILER_GCC && __GXX_EXPERIMENTAL_CXX0X__

// ���岻ͬƽ̨��wchar_t�ĶԵ��ڲ�����
#if defined(ICN_WCHAR_T_UTF16)
typedef char16_t	wchar_t_equal_type;
#elif defined(ICN_WCHAR_T_UTF32)
typedef char32_t	wchar_t_equal_type;
#elif defined(ICN_WCHAR_T_UTF8)
typedef char8_t		wchar_t_equal_type;
#else
#error "wchar_t type not defined!"
#endif // ICN_WCHAR_T_UTF16

#if defined(ICN_WCHAR_T_UNSIGNED)
#if defined(ICN_WCHAR_T_UTF16)
typedef uint16_t    wchar_t_int_type;
#elif defined(ICN_WCHAR_T_UTF32)
typedef uint32_t    wchar_t_int_type;
#elif defined(ICN_WCHAR_T_UTF8)
typedef uint8_t     wchar_t_int_type;
#else
#error "wchar_t type not defined!"
#endif // ICN_WCHAR_T_UTF16
#elif defined(ICN_WCHAR_T_SIGNED)
#if defined(ICN_WCHAR_T_UTF16)
typedef int16_t     wchar_t_int_type;
#elif defined(ICN_WCHAR_T_UTF32)
typedef int32_t     wchar_t_int_type;
#elif defined(ICN_WCHAR_T_UTF8)
typedef int8_t      wchar_t_int_type;
#else
#error "wchar_t type not defined!"
#endif // ICN_WCHAR_T_UTF16
#else
#error "wchar_t signed type not specified!"
#endif 

// ��ͬƽ̨�Ļ��з�
#if defined(ICN_OS_WIN)
#define ICN_LINE_END        "\r\n"
#define ICN_LINE_END_W      L"\r\n"
#elif defined(ICN_OS_MACOSX)
#define ICN_LINE_END        "\r"
#define ICN_LINE_END_W      L"\r"
#else // others
#define ICN_LINE_END        "\n"
#define ICN_LINE_END_W      L"\n"
#endif // others

// �ַ�����һЩposix���ݺ�
#ifdef ICN_COMPILER_MSVC
#ifndef strcasecmp
#define strcasecmp _stricmp
#endif // !strcasecmp
#ifndef strncasecmp
#define strncasecmp _strnicmp
#endif // !strncasecmp
#endif //ICN_COMPILER_MSVC

typedef struct IcnUserData
{
	void* userData;
	void (ICN_STDCALL* lpfnAddRefUserData)(void* userData);
	void (ICN_STDCALL* lpfnReleaseUserData)(void* userData);

} IcnUserData;


