#pragma once
#include <stdint.h>
#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace slib {
    class Intrinsics {
    public:
#if defined(_MSC_VER)
        static uint32_t bitScanForward(uint32_t value) {
            unsigned long ret;
            _BitScanForward(&ret, value);
            return ret;
        }
#elif defined(__GNUC__)
        static uint32_t bitScanForward(uint32_t value) {
            return __builtin_ctz(value);
        }
#endif
    };
}
