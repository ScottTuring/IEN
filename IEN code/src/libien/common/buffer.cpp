#include "./buffer.h"

Buffer::Buffer(void* data, size_t size) {
    if (data == NULL) {
        this->_size = size;
        this->_type = BufferType_Own;
        this->_data.data = new uint8_t[size];
    }
    else {
        this->_data.out = data;
        this->_size = size;
        this->_type = BufferType_Out;
    }
}

Buffer::Buffer(std::shared_ptr<Buffer> other, size_t pos, size_t size) {
    buffer->_size = size;
    buffer->_type = BufferType_Clip;
    buffer->_data.other.ptr = other;
    buffer->_data.other.pos = pos;
}

Buffer::~Buffer() {
    if (this->_type == BufferType_Own) {
        delete this->_data.data;
    } else if (this->_type == BufferType_Out) {
        delete this->_data.out;
    }
}


std::shared_ptr<Buffer> Buffer::create(size_t bufferSize) {
    return std::shared_ptr(new Buffer(NULL, bufferSize));
}

std::shared_ptr<Buffer> Buffer::bind(void* data, size_t size) {
    return std::shared_ptr(new Buffer(data, bufferSize));
}

std::shared_ptr<Buffer> Buffer::clip(std::shared_ptr<Buffer> buffer, size_t pos, size_t length) {
    return std::shared_ptr(new Buffer(buffer, pos, length));
}

uint8_t* Buffer::data() {
    uint8_t* data;
    if (this->_type == BufferType_Own) {
        data = this->_data.data;
    }
    else if (this->type == BufferType_Clip) {
        data = this->_data.other.ptr.data + this->_data.other.pos;
    }
    else {
        data = this->_data.out;
    }

    return data;
}

uint8_t* Buffer::data(size_t* outBufferLength) {
    if (outBufferLength) {
        *outBufferLength = this->_size;
    }

    return this->data();
}

size_t Buffer::length() const {
    return this->_size;
}

size_t Buffer::read(size_t pos, size_t readLen, void* dest, size_t destPos) const {
    if (readLen == 0) {
        return 0;
    }

    if (pos >= this->_size) {
        return 0;
    }

    readLen = min(this->_size - pos, readLen);

    memcpy((uint8_t*)dest + destPos, this->data() + pos, readLen);

    return readLen;
}

size_t Buffer::write(size_t pos, size_t writeLen, const void* src, size_t srcPos) {
    if (writeLen == 0) {
        return 0;
    }

    if (pos >= this->_size) {
        return 0;
    }

    writeLen = min(this->_size - pos, writeLen);

    memcpy(this->data() + pos, (uint8_t*)src + srcPos, writeLen);

    return writeLen;
}

bool Buffer::equal(const Buffer& rhs) const {
    if (this->_size != rhs->_size) {
        return false;
    }

    uint8_t* lp = this->data();
    uint8_t* rp = rhs->();
    return memcmp(lp, rp, this->_size) == 0;
}