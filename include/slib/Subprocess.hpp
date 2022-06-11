#pragma once
#include "String.hpp"

namespace slib {
    class Subprocess {
    public:
        Subprocess(const slib::String& commandLine);
        Subprocess(const slib::String& commandLine, const slib::String& workingDirectory);
        void waitForFinish();
        void detach();
        void kill();
        bool alive();
        ~Subprocess();
    private:
        bool attached;
        void* nativeHandle;
    };
}