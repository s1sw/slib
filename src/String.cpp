#include "../include/slib/String.hpp"
#include "../include/slib/MinMax.hpp"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

namespace slib {
    const size_t SSO_THRESHOLD = 14;

    String::String(char c) {
        sso = true;
        smallLen = 1;
        small[0] = c;
        small[1] = 0;
    }

    String::String(const char* cStr) {
        size_t length = strlen(cStr);

        if (length < SSO_THRESHOLD) {
            sso = true;
            strncpy_s(small, cStr, length);
            small[SSO_THRESHOLD - 1] = '\0';
            smallLen = length;
        } else {
            sso = false;
            data = _strdup(cStr);
            len = length;
        }
    }

    String::String(const String& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, 14);
            smallLen = other.smallLen;
        } else {
            data = _strdup(other.data);
            len = other.len;
        }
    }

    String::String(String&& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, SSO_THRESHOLD);
            smallLen = other.smallLen;
        } else {
            data = other.data;
            len = other.len;
            other.data = nullptr;
        }
    }

    String::String() : sso(true), smallLen(0) {
        memset(small, 0, SSO_THRESHOLD);
    }

    size_t String::ByteLength() const {
        // sanity check
        if (sso)
            assert(smallLen < SSO_THRESHOLD);
        return sso ? smallLen : len;
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

    String String::Substring(size_t index, size_t count) {
        size_t actualCount = min(count, ByteLength() - index);
        assert(index < ByteLength());

        if (actualCount < SSO_THRESHOLD) {
            // Can do this without allocating
            String st;
            st.smallLen = actualCount;
            memcpy(st.small, Data() + index, actualCount);

            return st;
        } else {
            char* buf = (char*)malloc(actualCount + 1);
            assert(buf);
            buf[actualCount] = '\0';

            String st;
            st.sso = false;
            st.data = buf;
            st.len = actualCount;

            memcpy(buf, Data() + index, actualCount);
            return st;
        }
    }

    void String::Clear() {
        if (!sso) {
            free(data);
        }

        sso = true;
        smallLen = 0;
        memset(small, 0, SSO_THRESHOLD);
    }

    String String::operator+(const String& other) const {
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

        sso = totalLength < SSO_THRESHOLD;
        len = totalLength;

        if (sso) {
            memcpy(small, buf, totalLength + 1);
            smallLen = totalLength;
        } else {
            data = buf;
        }
    }

    void String::operator=(const String& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, SSO_THRESHOLD);
            smallLen = other.smallLen;
        } else {
            data = _strdup(other.data);
            len = other.len;
        }
    }

    // Simple character-by-character equality check
    bool String::operator==(const String& other) const {
        if (other.ByteLength() != ByteLength()) return false;

        size_t len = ByteLength();
        char* dataA = Data();
        char* dataB = Data();

        for (size_t i = 0; i < len; i++) {
            if (dataA[i] != dataB[i])
                return false;
        }

        return true;
    }

    String::Iterator String::Begin() {
        return String::Iterator{ *this, 0 };
    }

    String::Iterator String::begin() {
        return String::Iterator{ *this, 0 };
    }

    String::Iterator String::End() {
        return String::Iterator{ *this, ByteLength() };
    }

    String::Iterator String::end() {
        return String::Iterator{ *this, ByteLength() };
    }
}