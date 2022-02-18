#pragma once


typedef struct {
    uv_loop_t loop;
    uv_async_t* proxy;
    void (*activeCallBack)();
} _icnSysMessageLoop;

typedef uv_timer_t _icnSysMessageLoopTimer;


//////////////////////////////////////////////////////////////////////////
// loop
_icnSysMessageLoop* _newSysLoop();
void _deleteSysLoop(_icnSysMessageLoop* loop);

int _sysLoopRun(_icnSysMessageLoop* loop);
int _sysLoopStop(_icnSysMessageLoop* loop);

void _sysLoopSetActiveCallBack(_icnSysMessageLoop* loop, void (*cb)());
int _sysLoopActive(_icnSysMessageLoop* loop);


//////////////////////////////////////////////////////////////////////////
// timer

void _initSysTimer(_icnSysMessageLoop* loop, _icnSysMessageLoopTimer* timer);
void _uninitSysTimer(_icnSysMessageLoopTimer* timer, void (*cb)(_icnSysMessageLoopTimer*));

int _startSysTimer(_icnSysMessageLoopTimer* timer, void (*onTimer)(_icnSysMessageLoopTimer* timer), int64_t timeout, bool once);
int _stopSysTimer(_icnSysMessageLoopTimer* timer);