#pragma once

namespace slib {
    class String {
    public:
        String(const char* cStr);
        String(const String& other);
        String(String&& other);
        String();

        char* Data() const { return sso ? (char*)small : data; }
        const char* CStr() const { return sso ? small : data; }
        // Please note that this doesn't include the null byte at the end of the string!
        size_t ByteLength() const;

        String operator+(const String& other);
        void operator+=(const String& other);
        void operator=(const String& other);

        char operator[](size_t idx) { return sso ? small[idx] : data[idx]; }
    private:
        bool sso;
        union {
            struct {
                char* data;
                size_t len;
            };

            struct {
                char small[15];
            };
        };
        
    };
}