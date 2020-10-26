#pragma once

namespace slib {
    template<char separator>
    class BasePath {
    public:

    };

    // This is meant to be usable for both virtual filesystems in games
    // and native filesystems, so we should support using either separator.
    // In a perfect world, everything would use / but unfortunately Windows exists.
    typedef BasePath<'/'> Path;
#ifdef _WIN32
    typedef BasePath<'\\'> NativePath;
#else
    typedef Path NativePath;
#endif
}