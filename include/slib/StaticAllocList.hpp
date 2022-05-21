#pragma once
#include <cassert>
#include <string.h>
#include "Iterator.hpp"
#include <initializer_list>

namespace std {
    struct input_iterator_tag;

    struct output_iterator_tag;

    struct forward_iterator_tag;

    struct bidirectional_iterator_tag;

    struct random_access_iterator_tag;
}

namespace slib {
    // "Dynamically sized" but with a maximum size.
    template <class V, size_t maxElements>
    class StaticAllocList {
    public:
        class Iterator : public IterBase<V> {
            size_t num;
            StaticAllocList<V, maxElements>* list;

        public:
            Iterator(StaticAllocList<V, maxElements>& list, size_t num = 0)
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

        StaticAllocList()
            : actualElements(0) {
        }

        template<size_t N>
        StaticAllocList(size_t maxElements, const V(&list)[N])
            : actualElements(N) {
            static_assert(N <= maxElements && "Cannot have a StaticAllocList with more initial elements than its maximum");

            size_t i = 0;
            for (auto& val : list) {
                data[i] = val;
                i++;
            }
        }

        StaticAllocList(const StaticAllocList& other) noexcept
            : actualElements(other.actualElements) {
            for (size_t i = 0; i < other.actualElements; i++) {
                data[i] = other.data[i];
            }
        }

        StaticAllocList(StaticAllocList&& other) noexcept
            : actualElements(other.actualElements){
            for (size_t i = 0; i < other.actualElements; i++) {
                data[i] = static_cast<V&&>(other.data[i]);
            }
        }

        size_t numElements() const {
            return actualElements;
        }
        
        size_t max() const {
            return maxElements;
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
        }

        Iterator begin() { return Iterator(*this); }
        Iterator end() { return Iterator(*this, actualElements); }

    private:
        size_t actualElements;
        V data[maxElements];
    };
}
