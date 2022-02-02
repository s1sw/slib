#pragma once

namespace slib {
    class DynamicLibrary {
    public:
        DynamicLibrary(const char* path);
        void* getFunctionPointer(const char* name);
        bool loaded() { return mod != nullptr; }
        ~DynamicLibrary();
    private:
        void* mod;
    };
}
