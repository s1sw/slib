#pragma once

namespace slib {
    template <typename V>
    class IterBase {

    };
    struct InputIteratorTag {};
    struct OutputIteratorTag {};

    struct ForwardIteratorTag : public InputIteratorTag {};
    struct BidirectionalIteratorTag : public ForwardIteratorTag {};
    struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};

    namespace detail {
        template<class iter>
        size_t distance(iter a, iter b, InputIteratorTag) {
            size_t result = 0;
            while (a != b) {
                ++a;
                ++result;
            }
            return result;
        }

        template<typename iter>
        size_t distance(const iter& a, const iter& b, RandomAccessIteratorTag) {
            return b - a;
        }

        template <typename iter>
        void advance(iter& it, size_t count, RandomAccessIteratorTag) {
            it += count;
        }

        template <typename iter>
        void advance(iter& it, size_t count, ForwardIteratorTag) {
            for (size_t i = 0; i < count; i++) {
                it++;
            }
        }
    }
    
    template <typename iter>
    size_t distance(const iter& a, const iter& b) {
        return detail::distance(a, b, typename iter::Category());
    }

    template <typename iter>
    void advance(iter& it, size_t count) {
        return detail::advance(it, count, typename iter::Category());
    }
}