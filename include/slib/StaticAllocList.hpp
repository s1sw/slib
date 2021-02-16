#pragma once
#include <cassert>
#include <string.h>
#include "Iterator.hpp"
#include <initializer_list>

namespace slib {
    template <class V>
    class StaticAllocList {
    public:
        StaticAllocList(size_t maxElements)
            : maxElements(maxElements)
            , actualElements(0)
            , data(nullptr) {
            data = new V[maxElements];
        }

        template<size_t N>
        StaticAllocList(size_t maxElements, const V(&list)[N])
            : maxElements(maxElements)
            , actualElements(N)
            , data(nullptr) {
            assert(N <= maxElements && "Cannot have a StaticAllocList with more initial elements than its maximum");
            data = static_cast<V*>(malloc(sizeof(V) * maxElements));

            size_t i = 0;
            for (auto& val : list) {
                data[i] = val;
                i++;
            }
        }

        StaticAllocList(const StaticAllocList& other) noexcept
            : actualElements(other.actualElements)
            , maxElements(other.maxElements)
            , data(nullptr) {
            data = static_cast<V*>(malloc(sizeof(V) * other.maxElements));
            memcpy(data, other.data, sizeof(V) * other.maxElements);
        }

        StaticAllocList(StaticAllocList&& other) noexcept
            : data(other.data)
            , maxElements(other.maxElements)
            , actualElements(other.actualElements){
            data = other.data;
            maxElements = other.maxElements;
            actualElements = other.actualElements;
            other.data = nullptr;
        }

        size_t numElements() const {
            return actualElements;
        }

        V& at(size_t idx) {
            assert(idx < actualElements);
            return data[idx];
        }

        V& operator[](size_t idx) {
            assert(idx < actualElements);
            return data[idx];
        }

        void add(const V& val) {
            if (actualElements == maxElements) {
                assert(false && "Tried to add to full StaticAllocList");
                return;
            }

            data[actualElements] = val;
            actualElements++;
        }

        void removeAt(size_t idx) {
            memcpy(data + idx, data + idx + 1, (actualElements - idx) * sizeof(V));
            actualElements--;
        }

        void removeFromStart(size_t numRemoved) {
            memcpy(data, data + numRemoved, (actualElements - numRemoved) * sizeof(V));
            actualElements -= numRemoved;
        }

        void removeFromEnd(size_t numRemoved) {
            actualElements -= numRemoved;
        }

        void clear() {
            actualElements = 0;
        }

        ~StaticAllocList() {
            if (data != nullptr)
                free(data);
        }

        class Iterator : public IterBase<V> {
            size_t num;
            StaticAllocList<V>& list;

        public:
            Iterator(StaticAllocList<V>& list, size_t num = 0)
                : num(num)
                , list(list) {
            }

            Iterator& operator++() {
                num++;
                return *this;
            }

            bool operator==(const Iterator& other) const { return num == other.num; }
            bool operator!=(const Iterator& other) const { return num != other.num; }
            bool operator>=(const Iterator& other) const { return num >= other.num; }
            bool operator> (const Iterator& other) const { return num > other.num; }
            bool operator<=(const Iterator& other) const { return num <= other.num; }
            bool operator< (const Iterator& other) const { return num < other.num; }

            V& operator*() {
                return list[num];
            }

            size_t operator-(const Iterator& other) const {
                return num - other.num;
            }

            void operator+=(size_t amt) {
                num += amt;
            }

            size_t getIndex() const {
                return num;
            }

            using DifferenceType = size_t;
            using ValueType = V;
            using Pointer = V*;
            using Reference = V&;
            using IteratorCategory = RandomAccessIteratorTag;
            static RandomAccessIteratorTag Category() { return IteratorCategory{}; }
        };

        Iterator begin() { return Iterator(*this); }
        Iterator end() { return Iterator(*this, actualElements); }

    private:
        size_t maxElements;
        size_t actualElements;
        V* data;
    };
}
