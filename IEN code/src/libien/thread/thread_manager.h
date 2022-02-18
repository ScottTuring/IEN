#pragma once
#include "./thread_info.h"
#include "./thread_proxy.h"

typedef struct {
	uv_mutex_t lock;

	IcnList threads;
} _ThreadManager;

_ThreadManager* _getThreadManger();

int _threadManagerAdd(_ThreadInfo* info);
int _threadManagerDelete(_ThreadInfo* info);

// ��ȡָ���̶߳�Ӧ��proxy
_ThreadMessageProxy* _getThreadProxy(_ThreadInfo* info);