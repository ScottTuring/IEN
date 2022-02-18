#include "../common/common.h"
#include "./thread.h"
#include "./thread_proxy.h"

static void _threadProxyInit(_ThreadMessageProxy* proxy, _IOThreadInfo* info) {
    proxy->ref = 1;
    proxy->info = info;
    _threadInfoAddRef((_ThreadInfo*)info);

    proxy->msgManager = info->msgManager;
    _messageManagerAddRef(proxy->msgManager);
}

static void _threadProxyUninit(_ThreadMessageProxy* proxy) {
    assert(proxy->info);
    _threadInfoRelease((_ThreadInfo*)proxy->info);
    proxy->info = NULL;

    assert(proxy->msgManager);
    _messageManagerRelease(proxy->msgManager);
    proxy->msgManager = NULL;
}

_ThreadMessageProxy* _newThreadProxy(_IOThreadInfo* info) {
    assert(info);
    _ThreadMessageProxy* proxy = (_ThreadMessageProxy*)malloc(sizeof(_ThreadMessageProxy));
    _threadProxyInit(proxy, info);

    return proxy;
}

int32_t _threadProxyAddRef(_ThreadMessageProxy* proxy) {
    assert(proxy->ref > 0);
    return IcnAtomicInc32(&proxy->ref);
}

int32_t _threadProxyRelease(_ThreadMessageProxy* proxy) {
    assert(proxy->ref > 0);
    int32_t ret = IcnAtomicDec32(&proxy->ref);
    if (ret == 0) {
        _threadProxyUninit(proxy);
        free(proxy);
    }

    return ret;
}

int _threadProxyPostMessage(_ThreadMessageProxy* proxy,
    IcnOnMessage onMessage, IcnUserData userData) {

    _messageManagerPostMsg(proxy->msgManager, onMessage, userData);

    int ret = _sysLoopActive(proxy->info->loop);
    if (ret != 0) {
        return ICN_RESULT_FAILED;
    }

    return 0;
}