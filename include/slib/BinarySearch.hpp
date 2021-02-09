#pragma once
#include "Iterator.hpp"

namespace slib {
    namespace detail {
        // Original end is passed in so we can return it when the value is missing
        // TODO: Are all of the checks that return origEnd really necessary?
        template<typename Iter, typename V>
        Iter binarySearch(V target, Iter begin, Iter end, Iter origEnd) {
            if (begin >= end) {
                return origEnd;
            }

            size_t dist = distance(begin, end);

            if (dist == 1)
                return origEnd;

            Iter mid = begin;
            advance(mid, dist / 2);

            if (*mid == target) {
                return mid;
            } else if (*mid < target) {
                return binarySearch(target, mid, end, origEnd);
            } else if (*mid > target) {
                return binarySearch(target, begin, mid, origEnd);
            }
        }
    }

    template<typename Iter, typename V>
    Iter binarySearch(V target, Iter begin, Iter end) {
        return detail::binarySearch(target, begin, end, end);
    }

    template<typename Container, typename V>
    typename Container::Iterator binarySearch(V target, Container& container) {
        return detail::binarySearch(target, container.begin(), container.end(), container.end());
    }
}