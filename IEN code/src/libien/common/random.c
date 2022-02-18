#include "./basic_type.h"
#include "./random.h"

#if defined(ICN_OS_WIN)
#include "./random_win.inl"
#else
#include "./random_posix.inl"
#endif

uint64_t random64(uint64_t maxValue) {

    // Ϊ��ȡģʱ��ȷ������[0, maxValue)֮���������Ǿ��ȵ�
    const uint64_t most = (UINT64_MAX / maxValue) * maxValue - 1;

    uint64_t value;
    do {
        value = _random64();
    } while (value > most);

    return (value % maxValue);
}


// ��ȡһ��[begin, end]����������
int32_t randomRange32(int32_t begin, int32_t end) {

    if (begin == end) {
        return begin;
    }

    int32_t result = begin + (int32_t)random64((uint64_t)end - begin + 1);

    return result;
}

#ifdef ICN_OS_WIN
void randomBytes(uint8_t* lpBuffer, size_t size) {
    size_t filled = 0;
    while (filled < size) {
        uint64_t value = _random64();
        memcpy(lpBuffer + filled, &value, min(sizeof(value), size - filled));
        filled += sizeof(value);
    }
}
#endif // ICN_OS_WIN