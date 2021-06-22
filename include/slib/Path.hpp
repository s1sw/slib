#pragma once
#include "String.hpp"

namespace slib {
    class Path {
    public:
        Path(String pathStr);
        Path(const Path& other);
        bool isAbsolute() const;
        String fileExtension();
        // Returns either the file name without extension or the name of the final directory.
        String stem();
        void normalize();
        Path parentPath() const;
        const char* cStr() const;
        void replaceSeparator(char newSeparator);

        operator String() const;
    private:
        String pathStr;
    };
}
