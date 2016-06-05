#include "ieompp/iterators/integer_iterator.hpp"

namespace ieompp
{
    namespace iterators
    {
        template <typename T, bool const_iterator>
        IntegerIterator<T, const_iterator>::IntegerIterator(
            const typename IntegerIterator<T, const_iterator>::value_type& val)
            : _val(val)
        {
        }


        template <typename T, bool const_iterator>
        inline void
        IntegerIterator<T, const_iterator>::swap(IntegerIterator<T, const_iterator>& other)
        {
            std::swap(_val, other._val);
        }


        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::reference
            IntegerIterator<T, const_iterator>::
            operator*()
        {
            return _val;
        }

        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::value_type
            IntegerIterator<T, const_iterator>::
            operator[](const typename IntegerIterator<T, const_iterator>::difference_type& diff)
        {
            return _val + diff;
        }

        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::reference
            IntegerIterator<T, const_iterator>::
            operator++()
        {
            return ++_val;
        }

        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::reference
            IntegerIterator<T, const_iterator>::
            operator++(int)
        {
            return _val++;
        }

        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::reference
            IntegerIterator<T, const_iterator>::
            operator--()
        {
            return --_val;
        }

        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::reference
            IntegerIterator<T, const_iterator>::
            operator--(int)
        {
            return _val--;
        }

        template <typename T, bool const_iterator>
        inline IntegerIterator<T, const_iterator>& IntegerIterator<T, const_iterator>::
        operator+=(const typename IntegerIterator<T, const_iterator>::difference_type& diff)
        {
            _val += diff;
            return *this;
        }

        template <typename T, bool const_iterator>
        inline IntegerIterator<T, const_iterator>& IntegerIterator<T, const_iterator>::
        operator-=(const typename IntegerIterator<T, const_iterator>::difference_type& diff)
        {
            _val += diff;
            return *this;
        }

        template <typename T, bool const_iterator>
        inline IntegerIterator<T, const_iterator> IntegerIterator<T, const_iterator>::
        operator+(const typename IntegerIterator<T, const_iterator>::difference_type& diff) const
        {
            return IntegerIterator<T, const_iterator>(_val + diff);
        }

        template <typename T, bool const_iterator>
        inline IntegerIterator<T, const_iterator> IntegerIterator<T, const_iterator>::
        operator-(const typename IntegerIterator<T, const_iterator>::difference_type& diff) const
        {
            return IntegerIterator<T, const_iterator>(_val - diff);
        }

        template <typename T, bool const_iterator>
        inline typename IntegerIterator<T, const_iterator>::difference_type
            IntegerIterator<T, const_iterator>::
            operator-(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val - rhs._val;
        }

        template <typename T, bool const_iterator>
        inline bool IntegerIterator<T, const_iterator>::
        operator<(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val < rhs._val;
        }

        template <typename T, bool const_iterator>
        inline bool IntegerIterator<T, const_iterator>::
        operator>(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val > rhs._val;
        }

        template <typename T, bool const_iterator>
        inline bool IntegerIterator<T, const_iterator>::
        operator<=(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val <= rhs._val;
        }

        template <typename T, bool const_iterator>
        inline bool IntegerIterator<T, const_iterator>::
        operator>=(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val >= rhs._val;
        }

        template <typename T, bool const_iterator>
        inline bool IntegerIterator<T, const_iterator>::
        operator==(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val == rhs._val;
        }

        template <typename T, bool const_iterator>
        inline bool IntegerIterator<T, const_iterator>::
        operator!=(const IntegerIterator<T, const_iterator>& rhs) const
        {
            return _val != rhs._val;
        }


        template <typename T, bool const_iterator>
        inline IntegerIterator<T, const_iterator>
        operator+(const typename IntegerIterator<T, const_iterator>::difference_type& diff,
                  const IntegerIterator<T, const_iterator>& it)
        {
            return IntegerIterator<T, const_iterator>(diff + it._val);
        }

        template <typename T, bool const_iterator>
        inline IntegerIterator<T, const_iterator>
        operator-(const typename IntegerIterator<T, const_iterator>::difference_type& diff,
                  const IntegerIterator<T, const_iterator>& it)
        {
            return IntegerIterator<T, const_iterator>(diff - it._val);
        }
    }
}
