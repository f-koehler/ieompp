#ifndef IEOMPP_ITERATORS_INTEGER_ITERATOR_HPP_
#define IEOMPP_ITERATORS_INTEGER_ITERATOR_HPP_

#include <iterator>

namespace ieompp
{
    namespace iterators
    {
        template <typename T1, typename T2, bool value>
        struct use_first;

        template <typename T1, typename T2>
        struct use_first<T1, T2, true> {
            using type = T1;
        };

        template <typename T1, typename T2>
        struct use_first<T1, T2, false> {
            using type = T2;
        };

        template <typename T, bool const_iterator>
        class IntegerIterator
        {
            public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type        = T;
                using difference_type   = decltype(T() - T());
                using pointer           = typename use_first<const T*, T*, const_iterator>::type;
                using reference         = typename use_first<const T&, T&, const_iterator>::type;

                friend IntegerIterator<T, const_iterator>
                operator+(const IntegerIterator<T, const_iterator>&, const difference_type&);

                friend IntegerIterator<T, const_iterator>
                operator-(const IntegerIterator<T, const_iterator>&, const difference_type&);

            private:
                value_type _val;

            public:
                IntegerIterator(const value_type& val);
                IntegerIterator(const IntegerIterator&) = default;

                inline void swap(IntegerIterator& other);

                IntegerIterator& operator=(const IntegerIterator&) = default;
                reference operator*();
                value_type operator[](const difference_type& diff);
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

        template <typename T, bool const_iterator>
        IntegerIterator<T, const_iterator>
        operator+(const typename IntegerIterator<T, const_iterator>::difference_type& diff,
                  const IntegerIterator<T, const_iterator>& it);

        template <typename T, bool const_iterator>
        IntegerIterator<T, const_iterator>
        operator-(const typename IntegerIterator<T, const_iterator>::difference_type& diff,
                  const IntegerIterator<T, const_iterator>& it);
    }
}

#include "detail/integer_iterator_impl.hpp"

#endif
