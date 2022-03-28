#ifdef _WIN32
#include "../include/slib/DynamicLibrary.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace slib {
    DynamicLibrary::DynamicLibrary(const char* path) {
        mod = LoadLibraryA(path);
    }

    void* DynamicLibrary::getFunctionPointer(const char* name) {
        return (void*)GetProcAddress((HMODULE)mod, name);
    }

    DynamicLibrary::~DynamicLibrary() {
        FreeLibrary((HMODULE)mod);
    }
}
#endif
