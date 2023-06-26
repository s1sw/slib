#ifdef __linux__
#include "../include/slib/Subprocess.hpp"
#include <sys/types.h>

namespace slib {
    struct SubprocessNativeLinux {
        pid_t pid;
    };

    Subprocess::Subprocess(const String& commandLine) {
        // TODO
    }

    Subprocess::Subprocess(const String& commandLine, const String& workingDirectory) {
    }

    void Subprocess::waitForFinish() {
    }

    void Subprocess::detach() {
    }

    void Subprocess::kill() {
    }

    bool Subprocess::alive() {
        return false;
    }

    Subprocess::~Subprocess() {
    }
}
#endif
