#pragma once
#ifdef _WIN32
#include <functional>

namespace slib {
    class Win32Util {
    public:
        static void enumerateDriveLetters(void(*callback)(char, void*), void* data);
        static bool isFileHidden(const char* path);
    };
}
#endif