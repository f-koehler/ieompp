#include "hubbard/iterator_range.hpp"

template<typename Iterator>
IteratorRange<Iterator>::IteratorRange(Iterator begin, Iterator end)
    : m_begin(begin), m_end(end)
{
}

template<typename Iterator>
Iterator IteratorRange<Iterator>::begin() {
    return m_begin;
}

template<typename Iterator>
Iterator IteratorRange<Iterator>::end() {
    return m_end;
}
