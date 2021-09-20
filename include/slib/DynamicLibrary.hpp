#pragma once

namespace slib {
    class DynamicLibrary {
    public:
        DynamicLibrary(const char* path);
        void* getFunctionPointer(const char* name);
        ~DynamicLibrary();
    private:
        void* mod;
    };
}