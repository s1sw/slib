#pragma once
#include "String.hpp"

namespace slib {
    class Path {
    public:
        Path(String pathStr);
        bool IsAbsolute() const;
        String FileExtension();
        // Returns either the file name without extension or the name of the final directory.
        String Stem();
        void Normalize();
        Path ParentPath() const;
        const char* CStr() const;
        void ReplaceSeparator(char newSeparator);

        operator String() const;
    private:
        String pathStr;
    };
}