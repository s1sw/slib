#ifdef __linux__
#include "../include/slib/DynamicLibrary.hpp"
#include <dlfcn.h>

namespace slib {
    DynamicLibrary::DynamicLibrary(const char* path) {
        mod = dlopen(path, RTLD_LAZY);
    }

    void* DynamicLibrary::getFunctionPointer(const char* name) {
        return dlsym(mod, name);
    }

    DynamicLibrary::~DynamicLibrary() {
        if (mod != nullptr) {
            dlclose(mod);
        }
    }
}
#endif
