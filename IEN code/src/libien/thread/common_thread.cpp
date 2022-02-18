#include "../common/common.h"

#include "./thread.h"
#include "./msg_manager.h"
#include "./thread_info.h"
#include "./thread_manager.h"


static void _threadMain(_ThreadInfo* info) {
    assert(info->state == _ThreadState_init);
    info->state = _ThreadState_running;

    if (info->preMain) {
        int ret = info->preMain(info);
        if (ret != 0) {
            return;
        }
    }

    // 保存到当前线程tls
    _setThreadInfo(info);

    // 注册到线程管理器
    _threadManagerAdd(info);

    // 初始化完毕，触发事件
    assert(info->initLock);
    uv_sem_post(info->initLock);

    assert(info->main);
    info->exitCode = info->main(info->args);

    if (info->postMain) {
        info->postMain(info);
    }

    // 检查并执行退出事件
    if (info->atExitManager) {
        _execAtExits(info->atExitManager);
        _deleteAtExitManager(info->atExitManager);
        info->atExitManager = NULL;
    }

    // 从线程管理器移除
    _threadManagerDelete(info);

    // 重置当前线程tls
    _resetThreadInfo();

    assert(info->state == _ThreadState_running);
    info->state = _ThreadState_end;

    _threadInfoRelease(info);
}

int32_t IcnThreadAddRef(ICN_THREAD_HANDLE thandle) {
    return _threadInfoAddRef((_ThreadInfo*)thandle);
}

int32_t IcnThreadRelease(ICN_THREAD_HANDLE thandle) {
    return _threadInfoRelease((_ThreadInfo*)thandle);
}

bool IcnIsCurrentThread(ICN_THREAD_HANDLE thandle) {
    _ThreadInfo* info = _getCurrentThreadInfo();
    if (info == NULL) {
        return false;
    }

    return info == (_ThreadInfo*)thandle;
}

static int _startThread(_ThreadInfo* info, IcnThreadOptions options) {
    info->state = _ThreadState_init;

    /*
    uv_thread_options_t top = {
        .flags = UV_THREAD_NO_FLAGS
    };

    if (options.stackSize > 0) {
        top.flags = UV_THREAD_HAS_STACK_SIZE;
        top.stack_size = options.stackSize;
    }
    */

    uv_sem_t initSem;
    uv_sem_init(&initSem, 0);
    info->initLock = &initSem;

    int ret = uv_thread_create(&info->thread, (uv_thread_cb)_threadMain, info);
    if (ret != 0) {
        return ICN_RESULT_FAILED;
    }

    // 等待初始化完成
    uv_sem_wait(&initSem);
    uv_sem_destroy(&initSem);
    info->initLock = NULL;

    return 0;
}

int IcnCreateThread(IcnThreadMain main, void* args, IcnThreadOptions options, ICN_THREAD_HANDLE* thandle) {
    _ThreadInfo* info = _newThreadInfo(IcnThreadType_Native);
    info->main = main;
    info->args = args;

    int ret = _startThread(info, options);
    if (ret != 0) {
        _threadInfoRelease(info);
        if (thandle) {
            thandle = NULL;
        }
    } else {
        if (thandle) {
            *thandle = (ICN_THREAD_HANDLE)info;
            _threadInfoAddRef(info);
        }
    }

    return ret;
}


static void _onAsyncCallback() {
    _IOThreadInfo* info = (_IOThreadInfo*)_getCurrentThreadInfo();
    assert(info);

    while (_messageManagerDispatchMsg(info->msgManager)) {};
}

static int _ioThreadPreMain(void* args) {
    _IOThreadInfo* info = (_IOThreadInfo*)args;

    // 初始化消息循环
    info->loop = _newSysLoop();
    if (info->loop == NULL) {
        return ICN_RESULT_FAILED;
    }

    // 初始化消息管理器
    info->msgManager = _messageManagerNew();

    _sysLoopSetActiveCallBack(info->loop, _onAsyncCallback);

    return 0;
}

static void _ioThreadPostMain(void* args) {
    _IOThreadInfo* info = (_IOThreadInfo*)args;

    _deleteSysLoop(info->loop);
    info->loop = NULL;

    _messageManagerRelease(info->msgManager);
    info->msgManager = NULL;
}

static int _ioThreadMain(_IOThreadInfo* info) {

    // 暂不支持消息循环的嵌套
    _sysLoopRun(info->loop);

    return 0;
}

int IcnCreateIOThread(IcnThreadOptions options, ICN_THREAD_HANDLE* thandle) {
    _IOThreadInfo* info = (_IOThreadInfo*)_newThreadInfo(IcnThreadType_IO);
    info->base.main = (IcnThreadMain)_ioThreadMain;
    info->base.preMain = _ioThreadPreMain;
    info->base.postMain = _ioThreadPostMain;
    info->base.args = info;

    int ret = _startThread((_ThreadInfo*)info, options);
    if (ret != 0) {
        _threadInfoRelease((_ThreadInfo*)info);
        if (thandle) {
            *thandle = NULL;
        }
    } else {
        if (thandle) {
            *thandle = (ICN_THREAD_HANDLE)info;
            _threadInfoAddRef((_ThreadInfo*)info);
        }
    }

    return ret;
}

ICN_THREAD_HANDLE IcnGetCurrentThread() {
    _ThreadInfo* info = _getCurrentThreadInfo();
    if (info == NULL) {
        return NULL;
    }

    _threadInfoAddRef(info);

    return (ICN_THREAD_HANDLE)(info);
}

int IcnPostMessage(ICN_THREAD_HANDLE thandle, IcnOnMessage onMessage, IcnUserData userData) {
    _ThreadInfo* info = NULL;
    if (thandle == NULL) {
        info = _getCurrentThreadInfo();
    } else {
        info = (_ThreadInfo*)thandle;
    }

    if (info->type != IcnThreadType_IO) {
        return ICN_RESULT_FAILED;
    }

    _ThreadMessageProxy* proxy = _getThreadProxy(info);
    if (proxy == NULL) {
        return ICN_RESULT_FAILED;
    }

    int err = _threadProxyPostMessage(proxy, onMessage, userData);
    _threadProxyRelease(proxy);

    return err;
}

typedef struct DelayMessageUserData {
    IcnUserData udata;
    int64_t delayInMicroseconds;
    IcnOnMessage onMessage;
}DelayMessageUserData;

static DelayMessageUserData* _newDelayMessageUserData(int64_t delayInMicroseconds, IcnOnMessage onMessage, IcnUserData udata) {
    DelayMessageUserData* data = (DelayMessageUserData*)malloc(sizeof(DelayMessageUserData));
    data->delayInMicroseconds = delayInMicroseconds;
    data->udata = udata;
    data->onMessage = onMessage;
    if (udata.lpfnAddRefUserData) {
        udata.lpfnAddRefUserData(udata.userData);
    }

    return data;
}

static void _deleteDelayMessageUserData(DelayMessageUserData* data) {
    if (data->udata.lpfnReleaseUserData) {
        data->udata.lpfnReleaseUserData(data->udata.userData);
    }

    free(data);
}

static void _postDelayMessageCallBack(void* userData) {
    DelayMessageUserData *udata = (DelayMessageUserData*)userData;

    IcnSetTimeout(udata->delayInMicroseconds, (IcnTimerCallBack)udata->onMessage, udata->udata);

    _deleteDelayMessageUserData(udata);
}

int IcnPostDelayMessage(ICN_THREAD_HANDLE thandle, int64_t delayInMicroseconds, IcnOnMessage onMessage, IcnUserData userData) {
    DelayMessageUserData* data = _newDelayMessageUserData(delayInMicroseconds, onMessage, userData);

    IcnUserData udata;
    udata.userData = data;

    const int ret = IcnPostMessage(thandle, _postDelayMessageCallBack, udata);
    if (ret != 0) {
        _deleteDelayMessageUserData(data);
    }

    return ret;
}

int IcnThreadJoin(ICN_THREAD_HANDLE thandle) {
    _ThreadInfo* info = (_ThreadInfo*)thandle;
    assert(info->thread);

    int ret = uv_thread_join(&info->thread);
    if (ret != 0) {
        return ICN_RESULT_FAILED;
    }

    return ret;
}

int IcnThreadAtExit(IcnThreadExitCallBack proc, IcnUserData userData) {
    return _atExit(proc, userData);
}

static void _postQuitCallBack(void* args) {
    _IOThreadInfo* info = _getCurrentIOThreadInfo();
    assert(info);

    info->base.exitCode = (int)(intptr_t)args;

    _sysLoopStop(info->loop);
}

int IcnPostQuit(ICN_THREAD_HANDLE thandle, int code) {
    IcnUserData ud = { 0 };
    ud.userData = (void*)(intptr_t)code;

    return IcnPostMessage(thandle, _postQuitCallBack, ud);
}