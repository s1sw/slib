#pragma once
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "Iterator.hpp"
#include <iterator>

namespace slib {
    // Fixed-size container allocated on construction and freed on destruction.
    template <typename T>
    class HeapArray {
    public:
        HeapArray(size_t numElements) : numElements(numElements) {
            data = (T*)malloc(sizeof(T) * numElements);
            for (size_t i = 0; i < numElements; i++) {
                new (&data[i]) T; // placement new go brrr
            }
        }

        HeapArray(const HeapArray& other) {
            numElements = other.numElements;
            data = (T*)malloc(sizeof(T) * numElements);

            for (size_t i = 0; i < numElements; i++) {
                new (&data[i]) T{other.data[i]}; // placement new go brrr
            }
        }

        HeapArray(HeapArray&& other) {
            data = other.data;
            numElements = other.numElements;
            other.data = nullptr;
        }

        T& operator[](size_t idx) {
            assert(idx < numElements);
            return data[idx];
        }

        size_t size() const { return numElements; }

        class Iterator : public IterBase<T> {
            size_t num;
            HeapArray<T>* list;

        public:
            Iterator(HeapArray<T>& list, size_t num = 0)
                : num(num)
                , list(&list) {
            }

            void operator=(Iterator& it) {
                list = it.list;
                num = it.num;
            }

            Iterator& operator++() {
                num++;
                return *this;
            }

            Iterator& operator--() {
                num--;
                return *this;
            }

            bool operator==(const Iterator& other) const { return num == other.num; }
            bool operator!=(const Iterator& other) const { return num != other.num; }
            bool operator>=(const Iterator& other) const { return num >= other.num; }
            bool operator> (const Iterator& other) const { return num > other.num; }
            bool operator<=(const Iterator& other) const { return num <= other.num; }
            bool operator< (const Iterator& other) const { return num < other.num; }

            T& operator*() {
                return (*list)[num];
            }

            size_t operator-(const Iterator& other) const {
                return num - other.num;
            }

            void operator+=(size_t amt) {
                num += amt;
            }

            Iterator operator+(size_t amt) {
                return Iterator(*list, num + amt);
            }

            Iterator operator-(size_t amt) {
                return Iterator(*list, num - amt);
            }

            size_t getIndex() const {
                return num;
            }

            using DifferenceType = size_t;
            using ValueType = T;
            using Pointer = T*;
            using Reference = T&;
            using IteratorCategory = RandomAccessIteratorTag;

            // stl compat
            using difference_type = size_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;

            static RandomAccessIteratorTag Category() { return IteratorCategory{}; }
        };

        Iterator begin() { return Iterator(*this); }
        Iterator end() { return Iterator(*this, numElements); }

        ~HeapArray() {
            free(data);
        }
    private:
        T* data;
        const size_t numElements;
    };
}
