#pragma once

#include "./platform_specific.h"
#include "./result.h"

// GCC版本宏
#if defined(ICN_COMPILER_GCC)
#define ICN_GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)
#endif // ICN_COMPILER_GCC

// __COUNTER__的兼容宏
// GCC4.3 release才增加了__COUNTER__预定义宏的支持，所以在老的版本使用__LINE__替代，
// 所以在老版本gcc下使用者不要在同一行使用超过一个带__COUNTER__的检测宏！
#ifndef __COUNTER__
    #define __COUNTER__ __LINE__
#endif // !__COUNTER__

// size_t和ptrdiff_t是否是编译器内置类型
#if defined(ICN_OS_MACOSX) && defined(ICN_ARCH_CPU_64_BITS)
    #define ICN_SIZE_T_INNERTYPE        1
    #define ICN_PTRDIFF_T_INNERTYPE     1
#endif // ICN_OS_MACOSX


// 不同编译器下的stdcall调用规范定义
#ifndef ICN_STDCALL
	#if defined(ICN_COMPILER_MSVC)
		#define ICN_STDCALL __stdcall
	#elif defined(ICN_COMPILER_GCC)
        // https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#index-force_005falign_005farg_005fpointer-function-attribute_002c-x86
        // gcc 下，__stdcall__选项只在x86-32下起作用，64bits下关闭这个选项
        #if defined(ICN_ARCH_CPU_X86)
		    #define ICN_STDCALL __attribute__((__stdcall__))
        #else
            #define ICN_STDCALL 
        #endif
	#else
		#error "unknown complier!!"
	#endif // ICN_COMPILER_MSVC
#endif // ICN_STDCALL

// 工程里面使用extern "C"的统一宏
#ifndef ICN_EXTERN_C
	#ifdef __cplusplus	
		#define ICN_EXTERN_C extern "C"
        #define ICN_BEGIN_EXTERN_C extern "C"{
        #define ICN_END_EXTERN_C }
	#else
		#define ICN_EXTERN_C 
        #define ICN_BEGIN_EXTERN_C
        #define ICN_END_EXTERN_C
	#endif // __cplusplus
#endif // ICN_EXTERN_C


// 导出接口的相关定义
#if defined(ICN_COMPILER_MSVC)
    #if defined(ICN_STATIC)
            #define ICN_API(x) ICN_EXTERN_C x
	#elif defined(ICN_EXPORTS)
			#define ICN_API(x) ICN_EXTERN_C __declspec(dllexport) x /*__stdcall*/ 
	#else // ICN_EXPORTS
			#define ICN_API(x) ICN_EXTERN_C __declspec(dllimport) x /*__stdcall*/ 
	#endif // ICN_EXPORTS
#elif defined(ICN_COMPILER_GCC)
    #if defined(ICN_STATIC)
            #define ICN_API(x) ICN_EXTERN_C x
	#elif defined(ICN_EXPORTS)
			#define ICN_API(x) ICN_EXTERN_C __attribute__((__visibility__("default")/*, __stdcall__*/)) x
	#else // ICN_EXPORTS
			#define ICN_API(x) ICN_EXTERN_C __attribute__((__visibility__("default")/*, __stdcall__*/)) x 
	#endif // ICN_EXPORTS
#endif // complier


// 特殊属性相关定义
#ifdef ICN_COMPILER_GCC
    #define ICN_LOCAL   __attribute__ ((visibility ("hidden")))
    #define ICN_PUBLIC  __attribute__ ((visibility ("default")))
    #define ICN_PACK(exp)  exp  __attribute__((__packed__))
    #define ICN_PACK_STRUCT(name)  struct  __attribute__((__packed__)) name
    #define ICN_TYPEOF __typeof__
    #define ICN_INLINE __inline__
    #define ICN_THREAD_LOCAL __thread
#else // MSVC
    #define ICN_LOCAL
    #define ICN_PUBLIC
    #define ICN_PACK(exp) __pragma(pack(push, 1)) exp __pragma(pack(pop))
    #define ICN_PACK_STRUCT(name) __pragma(pack(push, 1)) struct name __pragma(pack(pop))
    #define ICN_TYPEOF typeof
    #define ICN_INLINE inline
    #define ICN_THREAD_LOCAL __declspec(thread)
#endif // MSVC


// 句柄类型的相关定义
#define ICN_DECLARE_HANDLE(name) struct __ICN_SAFE_HANDLE_##name { int unused; }; typedef struct __ICN_SAFE_HANDLE_##name *name;

#ifdef __cplusplus
	#define ICN_DECLARE_HANDLE_EX(name, base) struct __ICN_SAFE_HANDLE_##name : public __ICN_SAFE_HANDLE_##base { int n##name; }; typedef __ICN_SAFE_HANDLE_##name *name;
#else
	#define ICN_DECLARE_HANDLE_EX(name, base) ICN_DECLARE_HANDLE(name)
#endif //__cplusplus 


// 编译时函数名称宏
#if defined(ICN_COMPILER_GCC)
    #define ICN_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
    #define ICN_CURRENT_FUNCTION __FUNCSIG__
#elif defined(ICN_COMPILER_MSVC)
    #define ICN_CURRENT_FUNCTION __FUNCTION__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define ICN_CURRENT_FUNCTION __func__
#else // others
    #define ICN_CURRENT_FUNCTION "UnknownFunction"
#endif // others

// 用以中断的辅助宏
#if defined(_MSC_VER)
    #define ICN_DEBUGBREAK()  __debugbreak()
#else // GNUC
    #if defined(ICN_ARCH_CPU_ARM_FAMILY)
		#if defined(ICN_ARCH_CPU_32_BITS)
		#       define ICN_DEBUGBREAK() asm("bkpt 0")
		#   else // 64bits
		#       define ICN_DEBUGBREAK() asm("brk 0")
		#   endif // 64bits
    #elif defined(ICN_ARCH_CPU_MIPS_FAMILY)
	    #define ICN_DEBUGBREAK() asm("break 2")
    #elif defined(ICN_ARCH_CPU_X86_FAMILY)
	    #define ICN_DEBUGBREAK() asm("int3")
    #else
	    #error "unknown cpu arch!!"
    #endif //ARCH
#endif // GNUC


// 整型数组合辅助宏
#define ICN_MAKEUINT16(a, b)    ((uint16_t)(((uint8_t)(((uint32_t)(a)) & 0xFF)) | ((uint16_t)((uint8_t)(((uint32_t)(b)) & 0xFF))) << 8))
#define ICN_MAKEUINT32(a, b)    ((uint32_t)(((uint16_t)(((uint32_t)(a)) & 0xFFFF)) | ((uint32_t)((uint16_t)(((uint32_t)(b)) & 0xFFFF))) << 16))
#define ICN_MAKEUINT64(a, b)    ((uint64_t)(((uint32_t)(((uint64_t)(a)) & 0xFFFFFFFF)) | ((uint64_t)((uint32_t)(((uint64_t)(b)) & 0xFFFFFFFF))) << 32))

#define ICN_LOUINT32(x)         ((uint32_t)(((uint64_t)(x)) & 0xFFFFFFFF))
#define ICN_HIUINT32(x)         ((uint32_t)((((uint64_t)(x)) >> 32) & 0xFFFFFFFF))
#define ICN_LOUINT16(x)         ((uint16_t)(((uint32_t)(x)) & 0xFFFF))
#define ICN_HIUINT16(x)         ((uint16_t)((((uint32_t)(x)) >> 16) & 0xFFFF))
#define ICN_LOUINT8(x)          ((uint8_t)(((uint32_t)(x)) & 0xFF))
#define ICN_HIUINT8(x)          ((uint8_t)((((uint32_t)(x)) >> 8) & 0xFF))


// 检查一个返回值指针参数是否为空，
// 如果为空，返回ICN_RESULT_INVALID_POINTER;
// 否则设置默认返回值为NULL
#define ICN_VALIDATE_OUT_POINTER(x) \
    do {					        \
    TS_CHECK(x != NULL);	        \
    if (x == NULL)			        \
    return ICN_RESULT_INVALID_POINTER;\
    *x = NULL;				        \
    } while(0);

// 检查一个指针参数是否为空，
// 如果为空，返回ICN_RESULT_INVALID_POINT;
#define ICN_VALIDATE_POINTER(x)     \
    do {					        \
    TS_CHECK(x != NULL);	        \
    if (x == NULL)			        \
    return ICN_RESULT_INVALID_POINTER;	\
    } while(0);


// 禁止指定的warning
#if defined(ICN_COMPILER_MSVC)
    #define ICN_DISABLE_WARNING_BEGIN(warn) \
        __pragma(warning(push)) \
        __pragma(warning(disable : warn))
    #define ICN_DISABLE_WARNING_END() \
        __pragma(warning(pop))
#else // GNUC
    #define ICN_DISABLE_WARNING_BEGIN(warn) 
    #define ICN_DISABLE_WARNING_END()
#endif // GUNC

// 辅助宏
#define ICN_CONCAT_IMPL(x, y) x##y
#define ICN_MACRO_CONCAT(x, y) ICN_CONCAT_IMPL(x, y)

#define ICN_MACRO_STR(s) #s
#define ICN_MACRO_CONCAT_STR(x, y) ICN_MACRO_STR(x ## y)

#if ICN_GCC_VERSION >= 40200
    #define ICN_GCC_DIAGNOSTIC_DO_PRAGMA(x) _Pragma (#x)
    #define ICN_GCC_DIAGNOSTIC_PRAGMA(x) ICN_GCC_DIAGNOSTIC_DO_PRAGMA(GCC diagnostic x)
#endif //ICN_GCC_VERSION >= 40200

#if ICN_GCC_VERSION >= 40600
    #define ICN_DISABLE_GCCWARNING_BEGIN(warn) \
        ICN_GCC_DIAGNOSTIC_PRAGMA(push) \
        ICN_GCC_DIAGNOSTIC_PRAGMA(ignored ICN_MACRO_CONCAT_STR(-W, warn))
    #define ICN_DISABLE_GCCWARNING_END() \
        ICN_GCC_DIAGNOSTIC_PRAGMA(pop)

    #define ICN_DISABLE_GCCWARNING(warn) \
        ICN_GCC_DIAGNOSTIC_PRAGMA(ignored ICN_MACRO_CONCAT_STR(-W, warn))
    #define ICN_ENABLE_GCCWARNING(warn) \
        ICN_GCC_DIAGNOSTIC_PRAGMA(warning ICN_MACRO_CONCAT_STR(-W, warn))
#else // MSVN OR GCC under 4.6
    #define ICN_DISABLE_GCCWARNING_BEGIN(warn)
    #define ICN_DISABLE_GCCWARNING_END()

    #define ICN_DISABLE_GCCWARNING(warn)
    #define ICN_ENABLE_GCCWARNING(warn)
#endif //


// 数组长度
#define ICN_ARRAYSIZE(a)  (sizeof(a) / sizeof(*(a)))


// 计算成员变量大小
#define membersize(t, m) sizeof(((t*)(0))->m)



// 编译器指令优化相关宏
#ifdef ICN_COMPILER_GCC
    #define ICN_EXPECT(exp, value) __builtin_expect((exp), (value))
#else // MSVC
    #define ICN_EXPECT(exp, value) (exp)
#endif // MSVC

#define ICN_EXPECT_FALSE(exp) ICN_EXPECT(!!(exp), 0)
#define ICN_EXPECT_TRUE(exp)  ICN_EXPECT(!!(exp), 1)

#define ICN_LIKELY(exp)     ICN_EXPECT_TRUE(exp)
#define ICN_UNLIKELY(exp)   ICN_EXPECT_FALSE(exp)

/*
if (ICN_LIKELY(value)) 等价于if (value)，但是value为TRUE的可能性更大
if (ICN_UNLIKELY(value)) 也等价于if (value)，但是value为FASLE的可能性更大
使用上面两个宏用以“分支转移”的信息提供给编译器，编译器可以对代码进行优化，用以减少指令跳转带来的性能下降。

You should use it only in cases when the likeliest branch is very very very likely
, or when the unlikeliest branch is very very very unlikely.
*/

// 变量未使用警告
#ifdef ICN_COMPILER_GCC
#   define ICN_UNUSED_VARIABLE(x) UNUSED_ ## x __attribute__((__unused__))
#else // MSVC
#   define ICN_UNUSED_VARIABLE(x) (void)(x)
#endif //MSVC

