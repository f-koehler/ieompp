#ifndef IEOMPP_ITERATORS_INTEGER_ITERATOR_HPP_
#define IEOMPP_ITERATORS_INTEGER_ITERATOR_HPP_

#include <iterator>

namespace ieompp
{
    template <typename T>
    class IntegerIterator
    {
        public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = decltype(T() - T());
        using pointer           = T*;
        using reference         = T&;

        friend IntegerIterator<T> operator+(const IntegerIterator<T>&, const difference_type&);
        friend IntegerIterator<T> operator-(const IntegerIterator<T>&, const difference_type&);

        private:
        value_type _val;

        public:
        IntegerIterator(const value_type& val);
        IntegerIterator(const IntegerIterator&) = default;

        inline void swap(IntegerIterator& other);

        IntegerIterator& operator=(const IntegerIterator&) = default;
        reference operator*();
        reference operator[](const difference_type& diff);
        reference operator++();
        reference operator++(int);
        reference operator--();
        reference operator--(int);
        IntegerIterator& operator+=(const difference_type& diff);
        IntegerIterator& operator-=(const difference_type& diff);
        IntegerIterator operator+(const difference_type& diff) const;
        IntegerIterator operator-(const difference_type& diff) const;
        difference_type operator-(const IntegerIterator& rhs) const;
        bool operator<(const IntegerIterator& rhs) const;
        bool operator>(const IntegerIterator& rhs) const;
        bool operator<=(const IntegerIterator& rhs) const;
        bool operator>=(const IntegerIterator& rhs) const;
        bool operator==(const IntegerIterator& rhs) const;
        bool operator!=(const IntegerIterator& rhs) const;
    };

    template <typename T>
    IntegerIterator<T> operator+(const IntegerIterator<T>& it,
                                 const typename IntegerIterator<T>::difference_type& diff);

    template <typename T>
    IntegerIterator<T> operator-(const IntegerIterator<T>& it,
                                 const typename IntegerIterator<T>::difference_type& diff);
}

#include "detail/integer_iterator_impl.hpp"

#endif
