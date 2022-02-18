#include "../common/common.h"
#include "./thread.h"
#include "./thread_info.h"
#include <assert.h>


static uv_once_t _threadInfoTLS = UV_ONCE_INIT;
static uv_key_t _threadInfoKey;
static void _initThreadInfoTLS(void) {
    //assert(_threadInfoKey == NULL);
    uv_key_create(&_threadInfoKey);
}

void _setThreadInfo(_ThreadInfo* info) {
    uv_once(&_threadInfoTLS, _initThreadInfoTLS);
    assert(uv_key_get(&_threadInfoKey) == NULL);

    uv_key_set(&_threadInfoKey, info);
}

void _resetThreadInfo() {
    //assert(_threadInfoKey);

    uv_key_set(&_threadInfoKey, NULL);
}

_ThreadInfo* _newThreadInfo(IcnThreadType type) {
	size_t size;
	if (type == IcnThreadType_IO) {
		size = sizeof(_IOThreadInfo);
	}
	else {
		assert(type == IcnThreadType_Native);
		size = sizeof(_ThreadInfo);
	}

	_ThreadInfo* info = (_ThreadInfo*)malloc(size);
    memset(info, 0, size);

	info->ref = 1;
	info->type = type;
	
	return info;
}

int32_t _threadInfoAddRef(_ThreadInfo* info) {
	assert(info->ref > 0);
	return IcnAtomicInc32(&info->ref);
}

int32_t _threadInfoRelease(_ThreadInfo* info){
	assert(info->ref > 0);
	int32_t ref = IcnAtomicDec32(&info->ref);
	if (ref == 0) {
		free(info);
	}

	return ref;
}

_ThreadInfo* _getCurrentThreadInfo() {
    uv_once(&_threadInfoTLS, _initThreadInfoTLS);
    return (_ThreadInfo*)uv_key_get(&_threadInfoKey);
}

_IOThreadInfo* _getCurrentIOThreadInfo() {
    _ThreadInfo* info = _getCurrentThreadInfo();
    if (info == NULL) {
        return NULL;
    }

    if (info->type != IcnThreadType_IO) {
        return NULL;
    }

    return (_IOThreadInfo*)info;
}