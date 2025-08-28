#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator>
    class TakeIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;

        using value_type = typename std::iterator_traits<UnderlyingIterator>::value_type;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        TakeIterator(UnderlyingIterator iterator) : iterator_(iterator) {}

        TakeIterator operator++() {
            ++iterator_;

            return *this;
        }

        TakeIterator operator--() {
            --iterator_;

            return *this;
        }

        auto operator*() { return *iterator_; }

        TakeIterator& operator=(const TakeIterator& other) {
            iterator_ = other.iterator_;
        }

        bool operator==(const TakeIterator& other) { return iterator_ == other.iterator_; }

        bool operator!=(const TakeIterator& other) { return !(*this == other); }

        auto operator-(const TakeIterator& other) { return iterator_ - other.iterator_; }

    private:
        UnderlyingIterator iterator_;
    };

    template<typename Iterator>
    TakeIterator<Iterator> MakeTakeIterator(Iterator underlying_iterator) {
        return TakeIterator<Iterator>(underlying_iterator);
    }

    namespace view {

        class TakeAdaptor {
        public:
            explicit TakeAdaptor(size_t n) : n_(n) {}

            size_t GetN() const { return n_; }

        private:
            size_t n_;
        };

        TakeAdaptor take(size_t n) {
            return TakeAdaptor(n);
        }

        template<typename Range>
        IteratorRange<TakeIterator<typename Range::const_iterator>>
        operator|(Range const& range, TakeAdaptor adaptor) {

            return IteratorRange<TakeIterator<typename Range::const_iterator>>(
                    MakeTakeIterator(range.begin()),
                    MakeTakeIterator(std::next(range.begin(), adaptor.GetN())));
        }
    }
}