#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator>
    class DropIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename std::iterator_traits<UnderlyingIterator>::value_type;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        DropIterator(UnderlyingIterator iterator) :
                iterator_(iterator) {}

        DropIterator operator++() {
            ++iterator_;

            return *this;
        }

        DropIterator operator--() {
            --iterator_;

            return *this;
        }

        auto operator*() { return *iterator_; }

        DropIterator& operator=(const DropIterator& other) {
            iterator_ = other.iterator_;
        }

        bool operator==(const DropIterator& other) { return iterator_ == other.iterator_; }

        bool operator!=(const DropIterator& other) { return !(*this == other); }

        auto operator-(const DropIterator& other) { return iterator_ - other.iterator_; }

    private:
        UnderlyingIterator iterator_;
    };

    template<typename Iterator>
    DropIterator<Iterator> MakeDropIterator(Iterator underlying_iterator) {
        return DropIterator<Iterator>(underlying_iterator);
    }

    namespace view {

        class DropAdaptor {
        public:
            explicit DropAdaptor(size_t n) : n_(n) {}

            size_t GetN() const { return n_; }

        private:
            size_t n_;
        };

        DropAdaptor drop(size_t n) {
            return DropAdaptor(n);
        }

        template<typename Range>
        IteratorRange<DropIterator<typename Range::const_iterator>>
        operator|(Range const& range, DropAdaptor adaptor) {

            return IteratorRange<DropIterator<typename Range::const_iterator>>(
                    MakeDropIterator(std::next(range.begin(), adaptor.GetN())),
                    MakeDropIterator(range.end()));
        }
    }
}