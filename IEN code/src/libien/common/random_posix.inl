#include "./basic_type.h"
#include "./platform_headers.h"
#include "./random.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
    int fd;
}_PosixRandomFile;

static int _randomFD = -1;
static int _randomFileOpen() {

    int fd = open("/dev/urandom", O_RDONLY);

    return fd;
}

static uv_once_t randomFileInstance = UV_ONCE_INIT;

static void _randomFileInit(void) {
    _randomFD = _randomFileOpen();
}

static int _getRandomFile() {
    uv_once(&randomFileInstance, _randomFileInit);
    assert(_randomFD > 0);

    return _randomFD;
}

void randomBytes(uint8_t* dest, size_t size) {
    assert(dest && size > 0);

    int fd = _getRandomFile();

    // 可能需要连续读取多次
    size_t readLen = 0;
    while (readLen < size) {
        ssize_t ret = ICN_HANDLE_EINTR(read(fd, dest + readLen, size - readLen));
        if (ret <= 0) {
            break;
        }

        readLen += ret;
    }
}

static uint64_t _random64() {
    uint64_t result = 0;
    randomBytes((uint8_t*)&result, sizeof(result));

    return result;
}