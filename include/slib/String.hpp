#pragma once
#include "Iterator.hpp"

namespace slib {
    class String {
    public:
        String(char c);
        String(const char* cStr);
        String(const String& other);
        String(String&& other);
        String();

        char* data() const { return sso ? (char*)small : _data; }
        const char* cStr() const { return sso ? small : _data; }
        // Please note that this doesn't include the null byte at the end of the string!
        size_t byteLength() const;
        bool empty() const { return byteLength() == 0; }
        bool contains(char c) const;
        String substring(size_t index, size_t count = SIZE_MAX);
        void clear();

        String operator+(const String& other) const;
        void operator+=(const String& other);
        void operator=(const String& other);
        bool operator==(const String& other) const;
        bool operator==(const char* str) const;

        char& operator[](size_t idx) const { if (sso) return (char&)small[idx]; else return _data[idx]; }

        class Iterator : public IterBase<char> {
            size_t idx;
            String& str;

        public:
            Iterator(String& str, size_t idx = 0)
                : idx(idx)
                , str(str) {
            }

            Iterator(const Iterator& other) : idx(other.idx), str(other.str) {}

            Iterator& operator++() {
                idx++;
                return *this;
            }

            Iterator& operator--() {
                idx--;
                return *this;
            }

            bool operator==(const Iterator& other) { return idx == other.idx; }
            bool operator!=(const Iterator& other) { return idx != other.idx; }
            bool operator>=(const Iterator& other) { return idx >= other.idx; }
            bool operator>(const Iterator& other) { return idx > other.idx; }
            bool operator<=(const Iterator& other) { return idx <= other.idx; }
            bool operator<(const Iterator& other) { return idx < other.idx; }

            void operator=(const Iterator& other) {
                idx = other.idx;
                str = other.str;
            }

            char& operator*() {
                return str[idx];
            }

            size_t operator-(const Iterator& other) const {
                return idx - other.idx;
            }

            void operator+=(size_t amt) {
                idx += amt;
            }

            size_t getIndex() const {
                return idx;
            }

            using DifferenceType = size_t;
            using ValueType = char;
            using Pointer = char*;
            using Reference = char&;
            using IteratorCategory = RandomAccessIteratorTag;
            static RandomAccessIteratorTag Category() { return IteratorCategory{}; }
        };

        Iterator begin();
        Iterator end();
    private:
        bool sso;
        union {
            struct {
                char* _data;
                size_t len;
            };

            struct {
                unsigned char smallLen;
                char small[14];
            };
        };
    };
}
