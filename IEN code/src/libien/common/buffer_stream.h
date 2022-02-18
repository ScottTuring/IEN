/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once

#include "./basic_type.h"
#include "./buffer.h"

class BufferStream {
//默认使用主机序
public:
    void init(void* buffer, size_t length);
    size_t pos() const;
    size_t tail() const;
    int setPos(size_t newPos);

    int readUInt8(uint8_t* outResult);
    int writeUInt8(uint8_t v, size_t* pWriteBytes);

    int readInt8(int8_t* outResult);
    int writeInt8(int8_t v, size_t* pWriteBytes);

    int readUInt16(uint16_t* outResult);
    int writeUInt16(uint16_t v, size_t* pWriteBytes);

    int readInt16(int16_t* outResult);
    int writeInt16(int16_t v, size_t* pWriteBytes);

    int readUInt32(uint32_t* outResult);
    int writeUInt32(uint32_t v, size_t* pWriteBytes);

    int readInt32(int32_t* outResult);
    int writeInt32(int32_t v, size_t* pWriteBytes);

    int readUInt48(uint64_t* outResult);
    int writeUInt48(uint64_t v, size_t* pWriteBytes);

    int readUInt64(uint64_t* outResult);
    int writeUInt64(uint64_t v, size_t* pWriteBytes);

    int readInt64(int64_t* outResult);
    int writeInt64(int64_t v, size_t* pWriteBytes);

    int readByteArray(uint8_t* out, size_t length);
    int writeByteArray(const uint8_t* input, size_t length);

    int readBuffer(std::shared_ptr<Buffer>* hOutBuffer);
    int writeBuffer(const std::shared_ptr<Buffer>& buffer, size_t* pWriteBytes);

    uint8_t* buffer() {
        return this->_buffer;
    }

   

private:
    uint8_t* _buffer;
    size_t _length;
    size_t _pos;
};
