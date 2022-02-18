#pragma once

#include <uv.h>
#include "../common/common.h"

typedef struct {
    volatile int32_t ref;

    IcnList queue;
    uv_mutex_t lock;
} _MessageManager;

_MessageManager* _messageManagerNew();

int32_t _messageManagerAddRef(_MessageManager* manager);
int32_t _messageManagerRelease(_MessageManager* manager);

int _messageManagerDispatchMsg(_MessageManager* manager);
int _messageManagerPostMsg(_MessageManager* manager, IcnOnMessage onMessage,
    IcnUserData userData);