#include "./basic_type.h"
#include "./platform_headers.h"
#include "./buffer_stream.h"


void BufferStream::init(void* buffer, size_t length) {
    memset(this, 0, sizeof(BufferStream));
    this->_buffer = (uint8_t*)buffer;
    this->_length = length;
}

size_t BufferStream::pos() const {
    assert(this);
    return this->_pos;
}

size_t BufferStream::tail() const {
    assert(this);
    assert(this->_length >= this->_pos);

    return this->_length - this->_pos;
}

int BufferStream::setPos(size_t newPos) {
    assert(this);
    assert(newPos <= this->_length);
    this->_pos = newPos;
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readUInt8(uint8_t* outResult) {
    assert(this);
    assert(this->_pos + 1 <= this->_length);
    *outResult = *(uint8_t*)(this->_buffer + this->_pos);
    this->_pos += 1;
    return 1; //read 1 bytes
}

int BufferStream::writeUInt8(uint8_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 1 <= this->_length);
    *(uint8_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 1;
    *pWriteBytes = 1; //write 1 byte
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readInt8(int8_t* outResult) {
    assert(this);
    assert(this->_pos + 1 <= this->_length);
    *outResult = *(int8_t*)(this->_buffer + this->_pos);
    this->_pos += 1;
    return 1; //read 1 bytes
}


int BufferStream::writeInt8(int8_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 1 <= this->_length);
    *(int8_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 1;
    *pWriteBytes = 1; //write 1 byte
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readByteArray(uint8_t* outResult, size_t length) {
    assert(this);
    assert(this->_pos + length <= this->_length);
    memcpy(outResult, this->_buffer + this->_pos, length);
    this->_pos += length;
    return (int)length;
}

int BufferStream::writeByteArray(const uint8_t* input, size_t length) {
    assert(this);
    assert(this->_pos + length <= this->_length);
    memcpy(this->_buffer + this->_pos, input, length);
    this->_pos += length;

    return ICN_RESULT_SUCCESS;
}

int BufferStream::readUInt16(uint16_t* outResult) {
    assert(this);
    assert(this->_pos + 2 <= this->_length);
    *outResult = *(uint16_t*)(this->_buffer + this->_pos);
    this->_pos += 2;
    return 2; //read 2 bytes
}

int BufferStream::writeUInt16(uint16_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 2 <= this->_length);
    *(uint16_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 2;
    *pWriteBytes = 2; //write 2 byte
    return ICN_RESULT_SUCCESS;
}


int BufferStream::readInt16(int16_t* outResult) {
    assert(this);
    assert(this->_pos + 2 <= this->_length);
    *outResult = *(int16_t*)(this->_buffer + this->_pos);
    this->_pos += 2;
    return 2;
}

int BufferStream::writeInt16(int16_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 2 <= this->_length);
    *(int16_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 2;
    *pWriteBytes = 2; //write 2 byte
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readUInt32(uint32_t* outResult) {
    assert(this);
    assert(this->_pos + 4 <= this->_length);
    *outResult = *(uint32_t*)(this->_buffer + this->_pos);
    this->_pos += 4;
    return 4;
}

int BufferStream::writeUInt32(uint32_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 4 <= this->_length);
    *(uint32_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 4;
    *pWriteBytes = 4; //write 4 byte
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readInt32(int32_t* outResult) {
    assert(this);
    assert(this->_pos + 4 <= this->_length);
    *outResult = *(int32_t*)(this->_buffer + this->_pos);
    this->_pos += 4;
    return 4;
}

int BufferStream::writeInt32(int32_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 4 <= this->_length);
    *(int32_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 4;
    *pWriteBytes = 4; //write 4 byte
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readUInt48(uint64_t* outResult) {
	assert(this);
	assert(this->_pos + 6 <= this->_length);
	
	*outResult = *(uint64_t*)(this->_buffer + this->_pos) & 0x0000ffffffffffff;
	this->_pos += 6;
	return 6;
}

int BufferStream::writeUInt48(uint64_t v, size_t* pWriteBytes) {
	assert(this);
	assert(this->_pos + 6 <= this->_length);
	*(uint32_t*)(this->_buffer + this->_pos) = v & 0x00000000ffffffff;
	*(uint16_t*)(this->_buffer + this->_pos + 4) = (v >> 32) & 0x0000ffff;
	this->_pos += 6;
	*pWriteBytes = 6; //write 6 byte
	return ICN_RESULT_SUCCESS;
}


int BufferStream::readUInt64(uint64_t* outResult) {
    assert(this);
    assert(this->_pos + 8 <= this->_length);
    *outResult = *(uint64_t*)(this->_buffer + this->_pos);
    this->_pos += 8;
    return 8;
}

int BufferStream::writeUInt64(uint64_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 8 <= this->_length);
    *(uint64_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 8;
    *pWriteBytes = 8; //write 8 byte
    return ICN_RESULT_SUCCESS;
}

int BufferStream::readInt64(int64_t* outResult) {
    assert(this);
    assert(this->_pos + 8 <= this->_length);
    *outResult = *(int64_t*)(this->_buffer + this->_pos);
    this->_pos += 8;
    return 8;
}

int BufferStream::writeInt64(int64_t v, size_t* pWriteBytes) {
    assert(this);
    assert(this->_pos + 8 <= this->_length);
    *(int64_t*)(this->_buffer + this->_pos) = v;
    this->_pos += 8;
    *pWriteBytes = 8; //write 8 byte
    return ICN_RESULT_SUCCESS;
}


int BufferStream::readBuffer(std::shared_ptr<Buffer>* hOutBuffer) {
    assert(this);
    assert(this->_pos + 2 <= this->_length);
    uint16_t len;
    len = *(uint16_t*)(this->_buffer + this->_pos);
    this->_pos += 2;
    *hOutBuffer = NULL;

    if (len == 0)
    {
        return 2;
    }
    if (this->_pos + len > this->_length)
    {
        return -1;
    }

    *hOutBuffer = Buffer::create(len);
    size_t bufferLen;
    uint8_t* out = (*hOutBuffer)->data(&bufferLen);

    memcpy(out, this->_buffer + this->_pos, len);
    this->_pos += len;
    return 2 + len;
}

int BufferStream::writeBuffer(const std::shared_ptr<Buffer>& buffer, size_t* pWriteBytes) {
    assert(this);
    size_t length;
    uint8_t* input = buffer->data(&length);
    assert(length < 0xffff);
    assert(this->_pos + length + 2 <= this->_length);

    *(uint16_t*)(this->_buffer + this->_pos) = (uint16_t)length;
    this->_pos += 2;
    if (length > 0)
    {
        memcpy(this->_buffer + this->_pos, input, length);
        this->_pos += length;
    }
    *pWriteBytes = 2 + length;
    return ICN_RESULT_SUCCESS;
}

