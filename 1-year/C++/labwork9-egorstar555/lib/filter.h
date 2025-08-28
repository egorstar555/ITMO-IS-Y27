#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator, typename UnaryPredicate>
    class FilterIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename std::iterator_traits<UnderlyingIterator>::value_type;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        FilterIterator(UnderlyingIterator begin, UnderlyingIterator end, UnaryPredicate pred) :
                it_(begin), begin_(begin), end_(end), pred_(pred) {
            while (it_ != end_ && !pred_(*it_)) {
                ++it_;
            }
        }

        FilterIterator operator++() {
            if (it_ != end_) {
                ++it_;
                while (it_ != end_ && !pred_(*it_)) {
                    ++it_;
                }
            }

            return *this;
        }

        FilterIterator operator--() {
            if (it_ != begin_) {
                --it_;
                while (it_ != begin_ && !pred_(*it_)) {
                    --it_;
                }
            }

            return *this;
        }

        auto operator*() { return *it_; }

        FilterIterator& operator=(const FilterIterator& other) {
            it_ = other.it_;
            begin_ = other.begin_;
            end_ = other.end_;
            pred_ = other.pred_;
        }

        bool operator==(const FilterIterator& other) { return it_ == other.it_; }

        bool operator!=(const FilterIterator& other) { return it_ != other.it_; }

        auto operator-(const FilterIterator& other) { return it_ - other.it_; }

    private:
        UnderlyingIterator it_;
        UnderlyingIterator begin_;
        UnderlyingIterator end_;
        UnaryPredicate pred_;
    };

    template<typename Iterator, typename UnaryPredicate>
    FilterIterator<Iterator, UnaryPredicate>
    MakeFilterIterator(Iterator underlying_begin, Iterator underlying_end, UnaryPredicate pred) {
        return FilterIterator<Iterator, UnaryPredicate>(underlying_begin, underlying_end, pred);
    }

    namespace view {

        template<typename UnaryPredicate>
        class FilterAdaptor {
        public:
            explicit FilterAdaptor(UnaryPredicate pred) : pred_(pred) {}

            UnaryPredicate GetPredicate() const { return pred_; }

        private:
            UnaryPredicate pred_;
        };

        template<typename UnaryPredicate>
        FilterAdaptor<UnaryPredicate> filter(UnaryPredicate pred) {
            return FilterAdaptor<UnaryPredicate>(pred);
        }

        template<typename Range, typename UnaryPredicate>
        IteratorRange<FilterIterator<typename Range::const_iterator, UnaryPredicate>>
        operator|(Range const& range, FilterAdaptor<UnaryPredicate> adaptor) {

            return IteratorRange<FilterIterator<typename Range::const_iterator, UnaryPredicate>>(
                    MakeFilterIterator(range.begin(), range.end(), adaptor.GetPredicate()),
                    MakeFilterIterator(range.end(), range.end(), adaptor.GetPredicate()));
        }
    }
}
