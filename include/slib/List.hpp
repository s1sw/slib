#pragma once
#include <cassert>
#include "Iterator.hpp"
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <iterator>

namespace slib {
    // Dynamically sized container.
    template <class V>
    class List {
    public:
        class Iterator : public IterBase<V> {
            size_t num;
            List<V>* list;

        public:
            Iterator(const List<V>& list, size_t num = 0)
                : num(num)
                , list((List<V>*)& list) {
            }

            Iterator(const Iterator& other)
                : num(other.num)
                , list(other.list) {
            }

            void operator=(const Iterator& other) {
                list = other.list;
                num = other.num;
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
                return (*list)[num];
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

            using difference_type = size_t;
            using value_type = V;
            using pointer = V*;
            using reference = V&;
            using iterator_category = std::random_access_iterator_tag;
            static RandomAccessIteratorTag Category() { return IteratorCategory{}; }
        };

        List(size_t initialSize = 0, size_t allocIncrease = 16)
            : _data(nullptr)
            , actualElements(0)
            , allocatedElements(initialSize)
            , allocIncrease(allocIncrease) {
            if (initialSize == 0)
                return;
            growTo(initialSize);
        }

        List(const List& other)
            : actualElements(other.actualElements)
            , allocatedElements(other.allocatedElements)
            , allocIncrease(other.allocIncrease) {
            _data = new V[other.allocatedElements];

            for (size_t i = 0; i < other.actualElements; i++) {
                _data[i] = other._data[i];
            }
        }

        List(List&& other)
            : actualElements(other.actualElements)
            , allocatedElements(other.allocatedElements)
            , allocIncrease(other.allocIncrease)
            , _data(other._data) {
            other._data = nullptr;
        }

        List& operator=(List&& other) {
            allocIncrease = other.allocIncrease;
            actualElements = other.actualElements;
            allocatedElements = other.allocatedElements;
            _data = other._data;
            other._data = nullptr;
            return *this;
        }

        size_t numElements() {
            return actualElements;
        }

        void add(V&& val) {
            if (actualElements == allocatedElements) {
                growTo(allocatedElements + allocIncrease);
            }

            new (_data + actualElements) V{ std::move(val) };
            actualElements++;
        }

        void add(V& val) {
            if (actualElements == allocatedElements) {
                growTo(allocatedElements + allocIncrease);
            }

            new (_data + actualElements) V{ val };
            actualElements++;
        }

        V& at(size_t idx) {
            assert(idx < actualElements);
            return _data[idx];
        }

        V& operator[](size_t idx) {
            assert(idx < actualElements);
            return _data[idx];
        }

        bool contains(const V& val) {
            for (size_t i = 0; i < actualElements; i++) {
                if (at(i) == val)
                    return true;
            }

            return false;
        }

        void removeValue(const V& val) {
            size_t removeIdx = ~(size_t)(0);
            for (size_t i = 0; i < actualElements; i++) {
                if (at(i) == val) {
                    removeIdx = i;
                    break;
                }
            }

            if (removeIdx == ~(size_t)(0)) return;
            removeAt(removeIdx);
        }

        void removeRange(size_t start, size_t end) {
            for (size_t i = start; i < end; i++) {
                _data[i].~V();
            }

            memcpy(_data + start, _data + end, (actualElements - end) * sizeof(V));
            actualElements -= end - start;
        }

        void removeAt(size_t idx) {
            _data[idx].~V();
            memcpy(_data + idx, _data + idx + 1, (actualElements - idx) * sizeof(V));
            actualElements--;
        }

        void removeFromStart(size_t numRemoved) {
            for (size_t i = 0; i < numRemoved; i++) {
                _data[i].~V();
            }

            memcpy(_data, _data + numRemoved, (actualElements - numRemoved) * sizeof(V));
            actualElements -= numRemoved;
        }

        void removeFromEnd(size_t numRemoved) {
            actualElements -= numRemoved;
            V* start = _data + actualElements - 1;
            V* end = start + numRemoved;

            for (V* v = start; v < end; v++) {
                v->~V();
            }
        }

        void reserve(size_t minSize) {
            if (allocatedElements >= minSize) return;
            growTo(minSize);
        }

        void clear() {
            for (V* v = _data; v < _data + actualElements; v++) {
                v->~V();
            }

            actualElements = 0;
        }

        void erase(const Iterator& begin, const Iterator& end) {
            removeRange(begin.getIndex(), end.getIndex());
        }

        V* data() {
            return _data;
        }

        ~List() {
            delete[] _data;
        }

        Iterator begin() const { return Iterator(*this); }
        Iterator end() const { return Iterator(*this, actualElements); }
    private:
        void growTo(size_t targetElements) {
            V* newData = new V[targetElements];

            if constexpr (std::is_trivially_copyable_v<V>) {
                memcpy(newData, _data, actualElements * sizeof(V));
            } else {
                for (size_t i = 0; i < actualElements; i++) {
                    newData[i] = std::move(_data[i]);
                }
            }

            free(_data);

            _data = newData;
            allocatedElements = targetElements;
        }

        V* _data;
        size_t actualElements;
        size_t allocatedElements;
        size_t allocIncrease;
    };
}
