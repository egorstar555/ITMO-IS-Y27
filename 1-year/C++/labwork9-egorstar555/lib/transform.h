#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator, typename Function>
    class TransformIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::result_of_t<Function(typename std::iterator_traits<UnderlyingIterator>::value_type)>;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        TransformIterator(UnderlyingIterator iterator, Function func) : iterator_(iterator), func_(func) {}

        TransformIterator operator++() {
            ++iterator_;

            return *this;
        }

        TransformIterator operator--() {
            --iterator_;

            return *this;
        }


        auto operator*() { return func_(*iterator_); }

        TransformIterator& operator=(const TransformIterator& other) {
            iterator_ = other.iterator_;
            func_ = other.func_;
        }

        bool operator==(const TransformIterator& other) { return iterator_ == other.iterator_; }

        bool operator!=(const TransformIterator& other) { return !(*this == other); }

        auto operator-(const TransformIterator& other) { return iterator_ - other.iterator_; }

    private:
        UnderlyingIterator iterator_;
        Function func_;
    };

    template<typename Iterator, typename Function>
    TransformIterator<Iterator, Function> MakeTransformIterator(Iterator underlyingIterator, Function func) {
        return TransformIterator<Iterator, Function>(underlyingIterator, func);
    }

    namespace view {

        template<typename Function>
        class TransformAdaptor {
        public:
            explicit TransformAdaptor(Function func) : func_(func) {}

            Function GetFunction() const { return func_; }

        private:
            Function func_;
        };

        template<typename Function>
        TransformAdaptor<Function> transform(Function func) {
            return TransformAdaptor<Function>(func);
        }

        template<typename Range, typename Function>
        IteratorRange<TransformIterator<typename Range::const_iterator, Function>>
        operator|(Range const& range, TransformAdaptor<Function> adaptor) {

            return IteratorRange<TransformIterator<typename Range::const_iterator, Function>>(
                    MakeTransformIterator(range.begin(), adaptor.GetFunction()),
                    MakeTransformIterator(range.end(), adaptor.GetFunction()));
        }
    }
}
