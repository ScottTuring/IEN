#include "./thread.h"
#include "./thread_info.h"


typedef enum {
    _TimerState_Waiting = 0,
    _TimerState_During = 1,
    _TimerState_Cleared = 2,
} _TimerState;

typedef struct { 
    _icnSysMessageLoopTimer timer;

    IcnTimerCallBack cb;
    IcnUserData userData;
    bool once;
    _TimerState state;
}_TimerInfo;

static _TimerInfo* _newTimerInfo(IcnTimerCallBack cb, IcnUserData userData, bool once) {
    assert(cb);
    
    _TimerInfo* info = (_TimerInfo*)malloc(sizeof(_TimerInfo));
    memset(info, 0, sizeof(_TimerInfo));

    info->userData = userData;
    if (userData.lpfnAddRefUserData) {
        userData.lpfnAddRefUserData(userData.userData);
    }

    info->once = once;
    info->cb = cb;
    info->state = _TimerState_Waiting;

    return info;
}

static void _deleteTimerInfoImpl(_TimerInfo *info) {
    if (info->userData.lpfnReleaseUserData) {
        info->userData.lpfnReleaseUserData(info->userData.userData);
    }
    info->cb = NULL;

    free(info);
}

static void _uvCloseTimerCallBack(_icnSysMessageLoopTimer* timer) {
    _TimerInfo* info = (_TimerInfo*)timer->data;

    _deleteTimerInfoImpl(info);
}

static void _deleteTimerInfo(_TimerInfo* info) {
    _uninitSysTimer(&info->timer, _uvCloseTimerCallBack);
}

static void _timerCallBack(_icnSysMessageLoopTimer* timer) {
    _TimerInfo* info = (_TimerInfo*)timer->data;
    assert(info);

    assert(info->state == _TimerState_Waiting);
    info->state = _TimerState_During;

    assert(info->cb);
    info->cb(info->userData.userData);

    bool needClear = false;
    if (info->state == _TimerState_During) {
        if (info->once) {
            needClear = true;
        }
    } else if (info->state == _TimerState_Cleared) {
        needClear = true;
    }
    else {
       
    }

    if (needClear) {
        _stopSysTimer(timer);
        _deleteTimerInfo(info);
    }
}

static _TimerInfo* _setTimer(int64_t delayInMicroseconds, IcnTimerCallBack cb, IcnUserData userData, bool once) {
    _IOThreadInfo* info = _getCurrentIOThreadInfo();
    if (info == NULL) {
       
        return NULL;
    }

    _TimerInfo* timerInfo = _newTimerInfo(cb, userData, once);
    _initSysTimer(info->loop, &timerInfo->timer);

    timerInfo->timer.data = timerInfo;

    // 启动
    int ret = _startSysTimer(&timerInfo->timer, _timerCallBack, delayInMicroseconds, once);
    if (ret != 0) {
        _deleteTimerInfo(timerInfo);
        return NULL;
    }

    return timerInfo;
}


ICN_TIMER_HANDLE IcnSetImmediate(IcnTimerCallBack cb, IcnUserData userData) {
    _TimerInfo* info = _setTimer(0, cb, userData, true);

    return (ICN_TIMER_HANDLE)info;
}

ICN_TIMER_HANDLE IcnSetInterval(int64_t delayInMicroseconds, IcnTimerCallBack cb, IcnUserData userData) {
    _TimerInfo* info = _setTimer(delayInMicroseconds, cb, userData, false);

    return (ICN_TIMER_HANDLE)info;
}

ICN_TIMER_HANDLE IcnSetTimeout(int64_t delayInMicroseconds, IcnTimerCallBack cb, IcnUserData userData) {
    _TimerInfo* info = _setTimer(delayInMicroseconds, cb, userData, true);

    return (ICN_TIMER_HANDLE)info;
}

int IcnClearTimer(ICN_TIMER_HANDLE timer) {
    _TimerInfo* info = (_TimerInfo*)timer;

    // 同一个timer只可以clear一次！
    assert(info->state != _TimerState_Cleared);

    _TimerState cur = info->state;
    info->state = _TimerState_Cleared;

    // 如果是单次timer，并且在回调里面调用了clear，那么这里不负责清理，只修改状态
    if (cur == _TimerState_During) {
        return 0;
    } 

    _stopSysTimer(&info->timer);
    _deleteTimerInfo(info);

    return 0;
}