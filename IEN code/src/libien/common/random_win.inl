#define _CRT_RAND_S
#include <stdlib.h>
#include "./basic_type.h"
#include "./platform_headers.h"


static uint32_t _random32() {
    uint32_t value;
    rand_s(&value);

    return value;
}

static uint64_t _random64() {
    return ICN_MAKEUINT64(_random32(), _random32());
}