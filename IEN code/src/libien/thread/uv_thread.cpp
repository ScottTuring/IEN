#include <uv.h>
#include "../common/common.h"
#include "./thread.h"
#include "./thread_info.h"
#include "./sys_adapter.h"

int _uvTranslateError(int status) {
    if (status == 0) {
        return 0;
    }

    const char* msg = uv_err_name(status);
    printf("%s", msg);

    // TODO

    return -1;
}


//////////////////////////////////////////////////////////////////////////
// proxy

static void _onAsyncCallback(uv_async_t* proxy) {
    _icnSysMessageLoop* loop = (_icnSysMessageLoop*)proxy->data;

    loop->activeCallBack();
}

static uv_async_t* _newProxy(uv_loop_t* loop) {
    uv_async_t* proxy = (uv_async_t*)malloc(sizeof(uv_async_t));
    proxy->data = loop;

    int ret = uv_async_init(loop, proxy, _onAsyncCallback);
    if (ret != 0) {
        free(proxy);
        return NULL;
    }

    return proxy;
}


//////////////////////////////////////////////////////////////////////////
// loop

_icnSysMessageLoop* _newSysLoop() {
    _icnSysMessageLoop* loop = (_icnSysMessageLoop*)malloc(sizeof(_icnSysMessageLoop));
    int ret = uv_loop_init(&loop->loop);
    if (ret != 0) {
        free(loop);

        return NULL;
    }

    loop->loop.data = loop;

    loop->proxy = _newProxy(&loop->loop);
    if (loop->proxy == NULL) {
        _deleteSysLoop(loop);

        return NULL;
    }

    return loop;
}

static void _finalDeleteSysLoop(_icnSysMessageLoop* loop) {
    assert(loop->proxy == NULL);

    loop->activeCallBack = NULL;
    int err = uv_loop_close(&loop->loop);

    free(loop);
}

static void _deleteProxy(uv_async_t* proxy) {
    // _icnSysMessageLoop* sysLoop = (_icnSysMessageLoop*)((uv_loop_t*)proxy->data)->data;

    free(proxy);
}

static void _proxyCloseCallBack(uv_handle_t* proxy) {
    _deleteProxy((uv_async_t*)proxy);
}

void _deleteSysLoop(_icnSysMessageLoop* loop) {

    if (loop->proxy) {
        // 必须proxy完全关闭后，才可以close loop
        uv_close((uv_handle_t*)loop->proxy, _proxyCloseCallBack);
        loop->proxy = NULL;

        uv_run(&loop->loop, UV_RUN_NOWAIT);

        _finalDeleteSysLoop(loop);

    }
    else {
        _finalDeleteSysLoop(loop);
    }
}

int _sysLoopRun(_icnSysMessageLoop* loop) {
    int ret = uv_run(&loop->loop, UV_RUN_DEFAULT);
    if (ret != 0) {
        ret = _uvTranslateError(ret);
    }

    return ret;
}

int _sysLoopStop(_icnSysMessageLoop* loop) {
    uv_stop(&loop->loop);

    return 0;
}

void _sysLoopSetActiveCallBack(_icnSysMessageLoop* loop, void (*cb)()) {
    loop->activeCallBack = cb;
}

int _sysLoopActive(_icnSysMessageLoop* loop) {
    int ret = uv_async_send(loop->proxy);
    if (ret != 0) {
        ret = _uvTranslateError(ret);
    }

    return ret;
}


//////////////////////////////////////////////////////////////////////////
// uv timers

void _initSysTimer(_icnSysMessageLoop* loop, _icnSysMessageLoopTimer* timer) {
    uv_timer_init(&loop->loop, timer);
}

void _uninitSysTimer(_icnSysMessageLoopTimer* timer, void (*cb)(_icnSysMessageLoopTimer*)) {
    uv_close((uv_handle_t*)timer, (uv_close_cb)cb);
}

int _startSysTimer(_icnSysMessageLoopTimer* timer, void (*onTimer)(_icnSysMessageLoopTimer* timer), int64_t timeout, bool once) {

    // 转换到毫秒
    uint64_t delayInMicroseconds = timeout / 1000;
    if (timeout % 1000) {
        ++delayInMicroseconds;
    }

    uint64_t repeat;
    if (once) {
        repeat = 0;
    }
    else {
        repeat = delayInMicroseconds;
    }

    int ret = uv_timer_start(timer, (uv_timer_cb)onTimer, (uint64_t)delayInMicroseconds, repeat);
    if (ret != 0) {
        ret = _uvTranslateError(ret);
    }

    return ret;
}

int _stopSysTimer(_icnSysMessageLoopTimer* timer) {

    int ret = uv_timer_stop(timer);
    if (ret != 0) {
        ret = _uvTranslateError(ret);
    }

    return ret;
}




uv_loop_t* IcnThreadGetCurrentLoop() {
    _IOThreadInfo* info = (_IOThreadInfo*)_getCurrentIOThreadInfo();
    if (info == NULL) {
        return NULL;
    }

    return &info->loop->loop;
}