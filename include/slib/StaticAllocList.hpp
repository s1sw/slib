#pragma once
#include <cassert>
#include <string.h>
#include "Iterator.hpp"
#include <iterator>
#include <initializer_list>

namespace slib {
    // "Dynamically sized" but with a maximum size. Only makes one allocation.
    template <class V>
    class StaticAllocList {
    public:
        class Iterator : public IterBase<V> {
            size_t num;
            StaticAllocList<V>* list;

        public:
            Iterator(StaticAllocList<V>& list, size_t num = 0)
                : num(num)
                , list(&list) {
            }

            Iterator& operator=(const Iterator& it) {
                list = it.list;
                num = it.num;
                return *this;
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

            V& operator*() const {
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
            using ValueType = V;
            using Pointer = V*;
            using Reference = V&;
            using IteratorCategory = RandomAccessIteratorTag;

            // stl compat
            using difference_type = size_t;
            using value_type = V;
            using pointer = V*;
            using reference = V&;
            using iterator_category = std::random_access_iterator_tag;

            static RandomAccessIteratorTag Category() { return IteratorCategory{}; }
        };

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
            data = new V[maxElements];

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
            data = new V[maxElements];

            for (size_t i = 0; i < other.actualElements; i++) {
                data[i] = other.data[i];
            }
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

        void removeRange(size_t start, size_t end) {
            for (size_t i = start; i < end; i++) {
                data[i].~V();
            }

            memcpy(data + start, data + end, (actualElements - end) * sizeof(V));
            actualElements -= end - start;
        }

        void removeAt(size_t idx) {
            data[idx].~V();
            memcpy(data + idx, data + idx + 1, (actualElements - idx) * sizeof(V));
            actualElements--;
        }

        void removeFromStart(size_t numRemoved) {
            for (size_t i = 0; i < numRemoved; i++) {
                data[i].~V();
            }

            memcpy(data, data + numRemoved, (actualElements - numRemoved) * sizeof(V));
            actualElements -= numRemoved;
        }

        void removeFromEnd(size_t numRemoved) {
            for (size_t i = actualElements - numRemoved - 1; i < actualElements; i++) {
                data[i].~V();
            }

            actualElements -= numRemoved;
        }

        void erase(const Iterator& begin, const Iterator& end) {
            removeRange(begin.getIndex(), end.getIndex());
        }

        void clear() {
            actualElements = 0;
        }

        ~StaticAllocList() {
            if (data != nullptr)
                delete[] data;
        }

        Iterator begin() { return Iterator(*this); }
        Iterator end() { return Iterator(*this, actualElements); }

    private:
        size_t maxElements;
        size_t actualElements;
        V* data;
    };
}
