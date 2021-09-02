#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../include/slib/Win32Util.hpp"

namespace slib {
    void Win32Util::enumerateDriveLetters(void(*callback)(char, void*), void* data) {
        const char* driveLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        DWORD presentDrives = GetLogicalDrives();
        for (int i = 0; i < 26; i++) {
            bool isPresent = (presentDrives & (1 << i)) > 0;

            if (isPresent)
                callback(driveLetters[i], data);
        }
    }

    bool Win32Util::isFileHidden(const char* path) {
        DWORD attributes = GetFileAttributesA(path);

        return (attributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
    }
}
#endif