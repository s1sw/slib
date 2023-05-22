#pragma once
#include <stddef.h>
#include <stdint.h>
#include "Intrinsic.hpp"

namespace slib {
    template <const size_t numBits>
    class Bitset {
    public:
        class BitsetRef {
            friend class Bitset<numBits>;
            Bitset<numBits>& set;
            size_t index;
            BitsetRef(Bitset<numBits>& set, size_t index) : set(set), index(index) {}
        public:
            operator bool() const {
                return set.get(index);
            }

            BitsetRef& operator =(bool b) {
                set.set(index, b);
                return *this;
            }
        };

        Bitset() {
            for (size_t i = 0; i < numBytes; i++) {
                data[i] = 0;
            }
        }

        bool get(size_t idx) const {
            size_t dataIdx = idx / typeBits;
            size_t bitIdx = idx % typeBits;
            return (bool)((data[dataIdx] >> bitIdx) & 1);
        }

        void set(size_t idx, bool value) {
            size_t dataIdx = idx / typeBits;
            size_t bitIdx = idx % typeBits;

            uint32_t currentVal = data[dataIdx];
            uint32_t mask = ~(1 << bitIdx);

            data[dataIdx] = (currentVal & mask) | (value << bitIdx);
        }

        size_t count() const {
            size_t sum = 0;

            for (int i = 0; i < numBytes; i++) {
                sum += Intrinsics::countBits(data[i]);
            }

            return sum;
        }

        BitsetRef operator[](size_t idx) {
            return BitsetRef(*this, idx);
        }

        bool operator[](size_t idx) const {
            return get(idx);
        }
    private:
        using DataType = uint8_t;
        const static uint32_t typeBits = sizeof(DataType) * 8;
        const static uint32_t numBytes = (numBits + (typeBits - 1)) / typeBits;
        DataType data[numBytes];
    };
}
