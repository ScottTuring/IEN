/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <uv.h>
#include "../common/common.h"

typedef int (*IcnThreadMain)(void*);

ICN_DECLARE_HANDLE(ICN_THREAD_HANDLE);

// 线程类型
typedef enum {
    IcnThreadType_Native = 0,
    IcnThreadType_IO = 1,
} IcnThreadType;

typedef struct {
    char name[32];
    size_t stackSize;

    size_t ssize;
} IcnThreadOptions;

int32_t IcnThreadAddRef(ICN_THREAD_HANDLE thandle);
int32_t IcnThreadRelease(ICN_THREAD_HANDLE thandle);

// thandle是不是当前线程
bool IcnIsCurrentThread(ICN_THREAD_HANDLE thandle);

// 如果thandle不为空，那么返回句柄，外部使用完毕需要释放
int IcnCreateThread(IcnThreadMain main, void* args, IcnThreadOptions options, ICN_THREAD_HANDLE* thandle);
int IcnCreateIOThread(IcnThreadOptions options, ICN_THREAD_HANDLE* thandle);

typedef void (*IcnOnMessage)(void* lpUserData);

// 获取当先线程的句柄，不为NULL的话需要释放
ICN_THREAD_HANDLE IcnGetCurrentThread();

int IcnThreadJoin(ICN_THREAD_HANDLE thandle);

// 发送消息到指定线程
// thandle=NULL，则发送到当前线程
int IcnPostMessage(ICN_THREAD_HANDLE thandle, IcnOnMessage onMessage, IcnUserData userData);

int IcnPostDelayMessage(ICN_THREAD_HANDLE thandle, int64_t delayInMicroseconds, IcnOnMessage onMessage, IcnUserData userData);

int IcnPostQuit(ICN_THREAD_HANDLE thandle, int code);

// 注册当前线程的退出回调，退出时候会按照反序依次调用
typedef void (*IcnThreadExitCallBack)(void* userData);
int IcnThreadAtExit(IcnThreadExitCallBack proc, IcnUserData userData);

//#ifdef ICN_FRAMEWORK_UV

uv_loop_t* IcnThreadGetCurrentLoop();


//////////////////////////////////////////////////////////////////////////
// timer

ICN_DECLARE_HANDLE(ICN_TIMER_HANDLE);

typedef void (*IcnTimerCallBack)(void* userData);

ICN_TIMER_HANDLE IcnSetImmediate(IcnTimerCallBack cb, IcnUserData userData);
ICN_TIMER_HANDLE IcnSetInterval(int64_t delayInMicroseconds, IcnTimerCallBack cb, IcnUserData userData);
ICN_TIMER_HANDLE IcnSetTimeout(int64_t delayInMicroseconds, IcnTimerCallBack cb, IcnUserData userData);

// 同一个timer只可以clear一次！
// IcnSetImmediate和IcnSetTimeout触发后不可以再调用Clear
// 这里为了简化逻辑，没有使用引用计数的handle
int IcnClearTimer(ICN_TIMER_HANDLE timer);