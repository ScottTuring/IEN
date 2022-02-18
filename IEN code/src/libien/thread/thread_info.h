#pragma once

#include "./msg_manager.h"
#include "./at_exit.h"
#include "./sys_adapter.h"

typedef enum {
    _ThreadState_init = 0,
    _ThreadState_running = 1,
    _ThreadState_end = 2,
}_ThreadState;


typedef struct {
    volatile int32_t ref;
    IcnThreadType type;
    uv_thread_t thread;

    _ThreadState state;

    IcnThreadMain main;
    int (*preMain)(void*);
    void (*postMain)(void*);

    void* args;
    int exitCode;

    _AtExitManager* atExitManager;

    uv_sem_t* initLock;

} _ThreadInfo;

typedef struct {
    _ThreadInfo base;

    _icnSysMessageLoop* loop;

    _MessageManager *msgManager;

}_IOThreadInfo;


void _setThreadInfo(_ThreadInfo* info);
void _resetThreadInfo();

_ThreadInfo* _newThreadInfo(IcnThreadType type);

int32_t _threadInfoAddRef(_ThreadInfo* info);
int32_t _threadInfoRelease(_ThreadInfo* info);

// ��ȡ��ǰ�̵߳�ThreadInfo
_ThreadInfo* _getCurrentThreadInfo();
_IOThreadInfo* _getCurrentIOThreadInfo();