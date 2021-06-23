#include "../include/slib/String.hpp"
#include "../include/slib/MinMax.hpp"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define _CRT_SECURE_NO_WARNINGS
#ifndef _WIN32
#define _strdup strdup
#endif

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
            strncpy(small, cStr, length + 1);
            small[SSO_THRESHOLD - 1] = '\0';
            smallLen = length;
        } else {
            sso = false;
            _data = _strdup(cStr);
            len = length;
        }
    }

    String::String(const String& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, 14);
            smallLen = other.smallLen;
        } else {
            _data = _strdup(other._data);
            len = other.len;
        }
    }

    String::String(String&& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, SSO_THRESHOLD);
            smallLen = other.smallLen;
        } else {
            _data = other._data;
            len = other.len;
            other._data = nullptr;
        }
    }

    String::String() : sso(true), smallLen(0) {
        memset(small, 0, SSO_THRESHOLD);
    }

    size_t String::byteLength() const {
        // sanity check
        if (sso)
            assert(smallLen < SSO_THRESHOLD);
        return sso ? smallLen : len;
    }

    bool String::contains(char c) const {
        size_t len = byteLength();
        char* data = this->data();
        for (size_t i = 0; i < len; i++) {
            if (data[i] == c)
                return true;
        }

        return false;
    }

    String String::substring(size_t index, size_t count) {
        size_t actualCount = min(count, byteLength() - index);
        assert(index < byteLength());

        if (actualCount < SSO_THRESHOLD) {
            // Can do this without allocating
            String st;
            st.smallLen = actualCount;
            memcpy(st.small, data() + index, actualCount);

            return st;
        } else {
            char* buf = (char*)malloc(actualCount + 1);
            assert(buf);
            buf[actualCount] = '\0';

            String st;
            st.sso = false;
            st._data = buf;
            st.len = actualCount;

            memcpy(buf, data() + index, actualCount);
            return st;
        }
    }

    void String::clear() {
        if (!sso) {
            free(_data);
        }

        sso = true;
        smallLen = 0;
        memset(small, 0, SSO_THRESHOLD);
    }

    String String::operator+(const String& other) const {
        size_t totalLength = byteLength() + other.byteLength();

        char* buf = (char*)malloc(totalLength + 1);
        assert(buf);

        strncpy(buf, data(), byteLength());
        strncpy(buf + byteLength(), other.data(), other.byteLength());
        buf[totalLength] = '\0';

        return buf;
    }

    void String::operator+=(const String& other) {
        size_t totalLength = byteLength() + other.byteLength();

        char* buf = (char*)malloc(totalLength + 1);
        assert(buf);

        strncpy(buf, data(), byteLength());
        strncpy(buf + byteLength(), other.data(), other.byteLength());
        buf[totalLength] = '\0';

        if (!sso)
            free(_data);

        sso = totalLength < SSO_THRESHOLD;
        len = totalLength;

        if (sso) {
            memcpy(small, buf, totalLength + 1);
            smallLen = totalLength;
        } else {
            _data = buf;
        }
    }

    void String::operator=(const String& other) {
        sso = other.sso;

        if (sso) {
            memcpy(small, other.small, SSO_THRESHOLD);
            smallLen = other.smallLen;
        } else {
            _data = _strdup(other._data);
            len = other.len;
        }
    }

    // Simple character-by-character equality check
    bool String::operator==(const String& other) const {
        if (other.byteLength() != byteLength()) return false;

        size_t len = byteLength();
        char* dataA = data();
        char* dataB = data();

        for (size_t i = 0; i < len; i++) {
            if (dataA[i] != dataB[i])
                return false;
        }

        return true;
    }

    bool String::operator==(const char* str) const {
        return strcmp(str, data()) == 0;
    }

    String::Iterator String::begin() {
        return String::Iterator{ *this, 0 };
    }

    String::Iterator String::end() {
        return String::Iterator{ *this, byteLength() };
    }
}
