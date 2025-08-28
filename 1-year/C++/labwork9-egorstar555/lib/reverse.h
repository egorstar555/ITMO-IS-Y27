#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator>
    class ReverseIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename std::iterator_traits<UnderlyingIterator>::value_type;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit ReverseIterator(UnderlyingIterator iterator) : iterator_(--iterator) {
        }

        ReverseIterator operator++() {
            --iterator_;

            return *this;
        }

        ReverseIterator operator--() {
            ++iterator_;

            return *this;
        }

        auto operator*() { return *iterator_; }

        ReverseIterator& operator=(const ReverseIterator& other) {
            iterator_ = other.iterator_;
        }

        bool operator==(const ReverseIterator& other) { return iterator_ == other.iterator_; }

        bool operator!=(const ReverseIterator& other) { return !(*this == other); }

        auto operator-(const ReverseIterator& other) { return iterator_ - other.iterator_; }

    private:
        UnderlyingIterator iterator_;
    };

    template<typename Iterator>
    ReverseIterator<Iterator> MakeReverseIterator(Iterator underlying_iterator) {
        return ReverseIterator<Iterator>(underlying_iterator);
    }

    namespace view {

        class ReverseAdaptor {
        public:

        };

        ReverseAdaptor reverse() {
            return ReverseAdaptor();
        }

        template<typename Range>
        IteratorRange<ReverseIterator<typename Range::const_iterator >>
        operator|(Range const& range, ReverseAdaptor adaptor) {

            return IteratorRange<ReverseIterator<typename Range::const_iterator>>(
                    MakeReverseIterator(range.end()),
                    MakeReverseIterator(range.begin()));
        }
    }
}
