#pragma once

#include <iterator>
#include <type_traits>

template<typename Iterator>
struct IsConstIterator {
    static const bool value = std::is_const<
            typename std::remove_reference<
                    typename std::iterator_traits<
                            Iterator
                    >::reference
            >::type
    >::value;
};

template<bool is_const_iterator, typename Iterator>
class IteratorRangeImpl {
public:
    IteratorRangeImpl(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    Iterator begin() const { return begin_; }

    Iterator end() const { return end_; }

    typename std::iterator_traits<Iterator>::difference_type size() { return end_ - begin_; }

    using const_iterator = Iterator;
private:
    Iterator begin_;
    Iterator end_;
};

template<typename Iterator>
class IteratorRangeImpl<false, Iterator> {
public:
    IteratorRangeImpl(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    Iterator begin() const { return begin_; }

    Iterator end() const { return end_; }

    typename std::iterator_traits<Iterator>::difference_type size() { return end_ - begin_; }

    using iterator = Iterator;
private:
    Iterator begin_;
    Iterator end_;
};

template<typename Iterator>
class IteratorRange : public IteratorRangeImpl<IsConstIterator<Iterator>::value, Iterator> {
public:
    typedef IteratorRangeImpl<IsConstIterator<Iterator>::value, Iterator> base;

    IteratorRange(Iterator begin, Iterator end) : base(begin, end) {}
};


