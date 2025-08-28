#pragma once

#include "IteratorRange.h"

#include <iterator>
#include <map>
#include <type_traits>

namespace ranges {

    template<typename UnderlyingIterator>
    class KeysIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_type = typename std::iterator_traits<UnderlyingIterator>::value_type;
        using value_type = iterator_type::first_type;
        using difference_type = typename std::iterator_traits<UnderlyingIterator>::difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit KeysIterator(UnderlyingIterator iterator) : iterator_(iterator) {
        }

        KeysIterator operator++() {
            ++iterator_;

            return *this;
        }

        KeysIterator operator--() {
            --iterator_;

            return *this;
        }

        auto operator*() { return (*iterator_).first; }

        KeysIterator& operator=(const KeysIterator& other) {
            iterator_ = other.iterator_;
        }

        bool operator==(const KeysIterator& other) { return iterator_ == other.iterator_; }

        bool operator!=(const KeysIterator& other) { return !(*this == other); }

        auto operator-(const KeysIterator& other) { return iterator_ - other.iterator_; }

    private:
        UnderlyingIterator iterator_;
    };

    template<typename Iterator>
    KeysIterator<Iterator> MakeKeysIterator(Iterator underlying_iterator) {
        return KeysIterator<Iterator>(underlying_iterator);
    }

    namespace view {

        class KeysAdaptor {
        public:

        };

        KeysAdaptor keys() {
            return KeysAdaptor();
        }

        template<typename Range>
        IteratorRange<KeysIterator<typename Range::const_iterator >>
        operator|(Range const& range, KeysAdaptor adaptor) {

            static_assert(std::is_same<Range, std::map<typename Range::key_type, typename Range::mapped_type>>::value,
                          "Container type is not std::map");

            return IteratorRange<KeysIterator<typename Range::const_iterator>>(
                    MakeKeysIterator(range.begin()),
                    MakeKeysIterator(range.end()));
        }
    }
}