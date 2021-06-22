#pragma once
#include <cassert>
#include "Iterator.hpp"
#include <stdlib.h>
#include <string.h>

namespace slib {
    template <class V>
    class List {
    public:
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
            : allocIncrease(other.allocIncrease)
            , actualElements(other.actualElements)
            , allocatedElements(other.allocatedElements) {
            _data = static_cast<V*>(malloc(sizeof(V) * other.allocatedElements));
            memcpy(_data, other._data, sizeof(V) * other.allocatedElements);
        }

        List(List&& other)
            : allocIncrease(other.allocIncrease)
            , actualElements(other.actualElements)
            , allocatedElements(other.allocatedElements)
            , _data(other._data) {
            other._data = nullptr;
        }

        size_t numElements() {
            return actualElements;
        }

        void add(const V& val) {
            if (actualElements == allocatedElements) {
                growTo(allocatedElements + allocIncrease);
            }

            _data[actualElements] = val;
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

        V* data() {
            return _data;
        }

        ~List() {
            for (size_t i = 0; i < actualElements; i++) {
                _data[i].~V();
            }

            free(_data);
        }

        class Iterator : public IterBase<V> {
            size_t num;
            List<V>* list;

        public:
            Iterator(List<V>& list, size_t num = 0)
                : num(num)
                , list(&list) {
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
            static RandomAccessIteratorTag Category() { return IteratorCategory{}; }
        };

        Iterator begin() { return Iterator(*this); }
        Iterator end() { return Iterator(*this, actualElements); }
    private:
        void growTo(size_t targetElements) {
            V* newData = static_cast<V*>(malloc(sizeof(V) * targetElements));

            if (_data != nullptr) {
                memcpy(newData, _data, actualElements * sizeof(V));
                free(_data);
            }

            _data = newData;
            allocatedElements = targetElements;
        }

        V* _data;
        size_t actualElements;
        size_t allocatedElements;
        size_t allocIncrease;
    };
}
