
#pragma once

// 三个标准宏，用以区分当前的版本
// ICN_DEBUG           // 调试版
// ICN_RELEASE         // 日志版
// ICN_PRODUCTRELEASE  // 发布版
// ！！！上面三个宏不可同时定义！！！

// 使用者可以根据需要在外部定义对应的config.h，会通过下面三个步骤来确定include哪个标准config
// 1. 如果发现已经定义了ICN_CONFIG_HEADER宏，那么不再尝试include标准config文件
// 2. 如果发现已经定义了ICN_CONFIG_HEADER_INCLUDEFILE宏，那么会尝试include该头文件
// 3. 1和2都不符合，那么使用标准的config.h


// 如果没有定义任何一个预定义宏，那么根据编译器的DEBUG宏，来判断是不是调试版本，并定义ICN_DEBUG宏
#if !defined(ICN_DEBUG) || !defined(ICN_RELEASE) || !defined(ICN_PRODUCTRELEASE)
    #if defined(DEBUG) || defined(_DEBUG)
        #define ICN_DEBUG   1
    #endif // DEBUG
#endif

//如果不是调试版，那么根据判断是开发版(release)还是发布版(productrelease)
#ifndef ICN_DEBUG
    #define ICN_NODEBUG     1
    
    // 如果没有预定义ICN_RELEASE或者ICN_PRODUCTRELEASE宏，那么通过是否预定义了ICN_LOG判断是不是日志版Release
    #if !defined(ICN_RELEASE) || !defined(ICN_PRODUCTRELEASE)
        #ifdef ICN_LOG
            #define ICN_RELEASE         1
        #else // !ICN_LOG
            #define ICN_PRODUCTRELEASE  1
        #endif // !ICN_LOG
    #endif
    
    #if defined(ICN_RELEASE) && defined(ICN_PRODUCTRELEASE)
        #error  ICN_RELEASE and ICN_PRODUCTRELEASE defined at same time!
    #endif

#else // !ICN_DEBUG
    #if defined(ICN_RELEASE) || defined(ICN_PRODUCTRELEASE)
        #error ICN_RELEASE or ICN_PRODUCTRELEASE defined with ICN_DEBUG!
    #endif // ICN_RELEASE || ICN_PRODUCTRELEASE
#endif // !ICN_DEBUG


#ifndef ICN_CONFIG_HEADER
    #ifdef ICN_CONFIG_HEADER_INCLUDEFILE
        #include ICN_CONFIG_HEADER_INCLUDEFILE
    #else // include标准的config
        #if defined(ICN_DEBUG) || defined(ICN_RELEASE)
            //#include "./BaseReleaseConfig.h"
			
        #else
            //#include "./BaseProductReleaseConfig.h"
        #endif
    #endif
#endif // !ICN_CONFIG_HEADER

