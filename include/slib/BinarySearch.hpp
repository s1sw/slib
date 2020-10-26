#pragma once
#include "Iterator.hpp"

namespace slib {
    namespace detail {
        // Original end is passed in so we can return it when the value is missing
        // TODO: Are all of the checks that return origEnd really necessary?
        template<typename Iter, typename V>
        Iter BinarySearch(V target, Iter begin, Iter end, Iter origEnd) {
            if (begin >= end) {
                return origEnd;
            }

            size_t dist = Distance(begin, end);

            if (dist == 1)
                return origEnd;

            Iter mid = begin;
            Advance(mid, dist / 2);

            if (*mid == target) {
                return mid;
            } else if (*mid < target) {
                return BinarySearch(target, mid, end, origEnd);
            } else if (*mid > target) {
                return BinarySearch(target, begin, mid, origEnd);
            }
        }
    }

    template<typename Iter, typename V>
    Iter BinarySearch(V target, Iter begin, Iter end) {
        return detail::BinarySearch(target, begin, end, end);
    }

    template<typename Container, typename V>
    typename Container::Iterator BinarySearch(V target, Container& container) {
        return detail::BinarySearch(target, container.Begin(), container.End(), container.End());
    }
}