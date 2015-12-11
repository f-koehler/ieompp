#ifndef ITERATOR_RANGE_HPP_
#define ITERATOR_RANGE_HPP_

template<typename Iterator>
class IteratorRange {
    private:
        Iterator m_begin;
        Iterator m_end;

    public:
        IteratorRange(Iterator begin, Iterator end);
        Iterator begin();
        Iterator end();
};

#include "hubbard/iterator_range_impl.hpp"

#endif
