#include "../include/slib/String.hpp"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

namespace slib {
    String::String(const char* cStr) {
        size_t length = strlen(cStr);

        if (length <= 14) {
            sso = true;
            strncpy_s(small, cStr, length);
            small[14] = '\0';
        } else {
            sso = false;
            data = _strdup(cStr);
            len = length;
        }
    }

    String::String(const String& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, 15);
        } else {
            data = _strdup(other.data);
            len = other.len;
        }
    }

    String::String(String&& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, 15);
        } else {
            data = other.data;
            len = other.len;
            other.data = nullptr;
        }
    }

    String::String() : sso(true) {
        memset(small, 0, 15);
    }

    size_t String::ByteLength() const {
        return sso ? strlen(small) : len;
    }

    bool String::Contains(char c) const {
        size_t len = ByteLength();
        char* data = Data();
        for (size_t i = 0; i < len; i++) {
            if (data[i] == c)
                return true;
        }

        return false;
    }

    String String::operator+(const String& other) {
        size_t totalLength = ByteLength() + other.ByteLength();

        char* buf = (char*)malloc(totalLength + 1);
        assert(buf);

        strncpy_s(buf, totalLength + 1, Data(), ByteLength());
        strncpy_s(buf + ByteLength(), totalLength - ByteLength() + 1, other.Data(), other.ByteLength());
        buf[totalLength] = '\0';

        return buf;
    }

    void String::operator+=(const String& other) {
        size_t totalLength = ByteLength() + other.ByteLength();

        char* buf = (char*)malloc(totalLength + 1);
        assert(buf);

        strncpy_s(buf, totalLength + 1, Data(), ByteLength());
        strncpy_s(buf + ByteLength(), totalLength - ByteLength() + 1, other.Data(), other.ByteLength());
        buf[totalLength] = '\0';

        if (!sso)
            free(data);

        sso = totalLength < 15;
        len = totalLength;

        if (sso) {
            memcpy(small, buf, totalLength + 1);
        } else {
            data = buf;
        }
    }

    void String::operator=(const String& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, 15);
        } else {
            data = _strdup(other.data);
            len = other.len;
        }
    }
}