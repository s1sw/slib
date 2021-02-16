#include "slib/BufferReader.hpp"
#include <string.h>

namespace slib {
    BufferReader::BufferReader(size_t bufferSize, void* buffer) 
        : buffer {(char*)buffer}
        , bufferSize {bufferSize} 
        , offset {0} {
    }

    void BufferReader::readBytes(size_t numBytes, void* buf) {
        memcpy(buf, buffer + offset, numBytes);
        offset += numBytes;
    }

    size_t BufferReader::position() {
        return offset;
    }

    size_t BufferReader::size() {
        return bufferSize;
    }
}
