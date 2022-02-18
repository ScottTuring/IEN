/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <memory>
#include "./basic_type.h"


enum BufferType {
    BufferType_Own = 0,
    BufferType_Out = 1,
    BufferType_Clip = 2,
};

class Buffer {
public:
    static std::shared_ptr<Buffer> create(size_t bufferSize);
    static std::shared_ptr<Buffer> bind(void* data, size_t size);
    static std::shared_ptr<Buffer> clip(std::shared_ptr<Buffer> buffer, size_t pos, size_t length);

    uint8_t* data(size_t* outBufferLength);
    uint8_t* data();
    size_t length() const;
    size_t read(size_t pos, size_t readLen, void* dest, size_t destPos) const;
    size_t write(size_t pos, size_t writeLen, const void* src, size_t srcPos);
    bool equal(const Buffer& rhs) const;

private:
    Buffer(void* data, size_t size);
    Buffer(std::shared_ptr<Buffer> other, size_t pos, size_t size);
    ~Buffer();
private:
    BufferType _type;
    size_t _size;

    union {
        struct {
            std::shared_ptr<Buffer> ptr;
            size_t pos;
        } other;
        uint8_t* out;
        uint8_t* data;
    } _data;
};