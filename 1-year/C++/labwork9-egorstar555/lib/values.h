#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator>
    class ValuesIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_type = typename std::iterator_traits<UnderlyingIterator>::value_type;
        using value_type = iterator_type::second_type;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit ValuesIterator(UnderlyingIterator iterator) : iterator_(iterator) {
        }

        ValuesIterator operator++() {
            ++iterator_;

            return *this;
        }

        ValuesIterator operator--() {
            --iterator_;

            return *this;
        }

        auto operator*() { return (*iterator_).second; }

        ValuesIterator& operator=(const ValuesIterator& other) {
            iterator_ = other.iterator_;
        }

        bool operator==(const ValuesIterator& other) { return iterator_ == other.iterator_; }

        bool operator!=(const ValuesIterator& other) { return !(*this == other); }

        auto operator-(const ValuesIterator& other) { return iterator_ - other.iterator_; }

    private:
        UnderlyingIterator iterator_;
    };

    template<typename Iterator>
    ValuesIterator<Iterator> MakeValuesIterator(Iterator underlying_iterator) {
        return ValuesIterator<Iterator>(underlying_iterator);
    }

    namespace view {

        class ValuesAdaptor {
        public:

        };

        ValuesAdaptor values() {
            return ValuesAdaptor();
        }

        template<typename Range>
        IteratorRange<ValuesIterator<typename Range::const_iterator >>
        operator|(Range const& range, ValuesAdaptor adaptor) {

            static_assert(std::is_same<Range, std::map<typename Range::key_type, typename Range::mapped_type>>::value,
                          "Container type is not std::map");

            return IteratorRange<ValuesIterator<typename Range::const_iterator>>(
                    MakeValuesIterator(range.begin()),
                    MakeValuesIterator(range.end()));
        }
    }
}
