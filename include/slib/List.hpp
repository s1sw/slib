#pragma once
#include <cassert>
#include "Iterator.hpp"

namespace slib {
    template <class V>
    class List {
    public:
        List(size_t initialSize = 0, size_t allocIncrease = 16)
            : allocIncrease(allocIncrease)
            , actualElements(0)
            , allocatedElements(initialSize)
            , data(nullptr) {
            if (initialSize == 0)
                return;
            GrowTo(initialSize);
        }

        List(const List& other) 
            : allocIncrease(other.allocIncrease)
            , actualElements(other.actualElements)
            , allocatedElements(other.allocatedElements) {
            data = static_cast<V*>(malloc(sizeof(V) * other.allocatedElements));
            memcpy(data, other.data, sizeof(V) * other.allocatedElements);
        }

        List(List&& other)
            : allocIncrease(other.allocIncrease)
            , actualElements(other.actualElements)
            , allocatedElements(other.allocatedElements)
            , data(other.data) {
            other.data = nullptr;
        }

        size_t NumElements() {
            return actualElements;
        }

        void Add(const V& val) {
            if (actualElements == allocatedElements) {
                GrowTo(allocatedElements + allocIncrease);
            }

            data[actualElements] = val;
            actualElements++;
        }

        V& At(size_t idx) {
            assert(idx < actualElements);
            return data[idx];
        }

        V& operator[](size_t idx) {
            assert(idx < actualElements);
            return data[idx];
        }

        void RemoveAt(size_t idx) {
            memcpy(data + idx, data + idx + 1, (actualElements - idx) * sizeof(V));
            actualElements--;
        }

        void RemoveFromStart(size_t numRemoved) {
            memcpy(data, data + numRemoved, (actualElements - numRemoved) * sizeof(V));
            actualElements -= numRemoved;
        }

        void RemoveFromEnd(size_t numRemoved) {
            actualElements -= numRemoved;
        }

        ~List() {
            free(data);
        }

        class Iterator : public IterBase<V> {
            size_t num;
            List<V>& list;

        public:
            Iterator(List<V>& list, size_t num = 0)
                : num(num)
                , list(list) {
            }

            Iterator& operator++() {
                num++;
                return *this;
            }

            bool operator==(Iterator& other) { return num == other.num; }
            bool operator!=(Iterator& other) { return num != other.num; }
            bool operator>=(Iterator& other) { return num >= other.num; }
            bool operator>(Iterator& other) { return num > other.num; }
            bool operator<=(Iterator& other) { return num <= other.num; }
            bool operator<(Iterator& other) { return num < other.num; }


            V& operator*() {
                return list[num];
            }

            size_t operator-(const Iterator& other) const {
                return num - other.num;
            }

            void operator+=(size_t amt) {
                num += amt;
            }

            size_t GetIndex() const {
                return num;
            }

            using DifferenceType = size_t;
            using ValueType = V;
            using Pointer = V*;
            using Reference = V&;
            using IteratorCategory = ForwardIteratorTag;
            static InputIteratorTag Category() { return IteratorCategory{}; }
        };

        Iterator Begin() { return Iterator(*this); }
        Iterator End() { return Iterator(*this, actualElements); }

        // Define these as well so we can use
        // range based for loops
        Iterator begin() { return Begin(); }
        Iterator end() { return End(); }

    private:
        void GrowTo(size_t targetElements) {
            V* newData = static_cast<V*>(malloc(sizeof(V) * targetElements));

            if (data != nullptr) {
                memcpy(newData, data, actualElements * sizeof(V));
                free(data);
            }

            data = newData;
            allocatedElements = targetElements;
        }

        V* data;
        size_t actualElements;
        size_t allocatedElements;
        size_t allocIncrease;
    };
}