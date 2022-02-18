/*************************************************
Copyright��������ѧ�����о���Ժ PKUSZ
Author�����������������������������ص�ʵ���ң�ICNlab��
               IENС��
web��http://www.ienlab.com/
Description:������̬���磨Intelligent Eco Networking��
**************************************************/
#pragma once
#include <uv.h>
#include "../common/common.h"

typedef int (*IcnThreadMain)(void*);

ICN_DECLARE_HANDLE(ICN_THREAD_HANDLE);

// �߳�����
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

// thandle�ǲ��ǵ�ǰ�߳�
bool IcnIsCurrentThread(ICN_THREAD_HANDLE thandle);

// ���thandle��Ϊ�գ���ô���ؾ�����ⲿʹ�������Ҫ�ͷ�
int IcnCreateThread(IcnThreadMain main, void* args, IcnThreadOptions options, ICN_THREAD_HANDLE* thandle);
int IcnCreateIOThread(IcnThreadOptions options, ICN_THREAD_HANDLE* thandle);

typedef void (*IcnOnMessage)(void* lpUserData);

// ��ȡ�����̵߳ľ������ΪNULL�Ļ���Ҫ�ͷ�
ICN_THREAD_HANDLE IcnGetCurrentThread();

int IcnThreadJoin(ICN_THREAD_HANDLE thandle);

// ������Ϣ��ָ���߳�
// thandle=NULL�����͵���ǰ�߳�
int IcnPostMessage(ICN_THREAD_HANDLE thandle, IcnOnMessage onMessage, IcnUserData userData);

int IcnPostDelayMessage(ICN_THREAD_HANDLE thandle, int64_t delayInMicroseconds, IcnOnMessage onMessage, IcnUserData userData);

int IcnPostQuit(ICN_THREAD_HANDLE thandle, int code);

// ע�ᵱǰ�̵߳��˳��ص����˳�ʱ��ᰴ�շ������ε���
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

// ͬһ��timerֻ����clearһ�Σ�
// IcnSetImmediate��IcnSetTimeout�����󲻿����ٵ���Clear
// ����Ϊ�˼��߼���û��ʹ�����ü�����handle
int IcnClearTimer(ICN_TIMER_HANDLE timer);