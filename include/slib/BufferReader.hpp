#pragma once
#include <uchar.h>
#include <stdint.h>

namespace slib {
    class BufferReader {
    public:
        BufferReader(size_t bufferSize, void* buffer);
        void readBytes(size_t numBytes, void* buf);

        template <typename T>
        T readType() {
            T* ptr = (T*)&buffer[offset];
            offset += sizeof(T);
            return *ptr;
        }

        size_t position();
        size_t size();
    private:
        char* buffer;
        size_t bufferSize;
        size_t offset;
    };
}
