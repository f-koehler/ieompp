#include "ieompp/iterators/integer_iterator.hpp"

namespace ieompp
{
    template <typename T>
    IntegerIterator<T>::IntegerIterator(const typename IntegerIterator<T>::value_type& val)
        : _val(val)
    {
    }


    template <typename T>
    inline void IntegerIterator<T>::swap(IntegerIterator<T>& other)
    {
        std::swap(_val, other._val);
    }


    template <typename T>
    inline typename IntegerIterator<T>::reference IntegerIterator<T>::operator*()
    {
        return _val;
    }

    template <typename T>
    inline typename IntegerIterator<T>::reference IntegerIterator<T>::
    operator[](const typename IntegerIterator<T>::difference_type& diff)
    {
        return _val + diff;
    }

    template <typename T>
    inline typename IntegerIterator<T>::reference IntegerIterator<T>::operator++()
    {
        return ++_val;
    }

    template <typename T>
    inline typename IntegerIterator<T>::reference IntegerIterator<T>::operator++(int)
    {
        return _val++;
    }

    template <typename T>
    inline typename IntegerIterator<T>::reference IntegerIterator<T>::operator--()
    {
        return --_val;
    }

    template <typename T>
    inline typename IntegerIterator<T>::reference IntegerIterator<T>::operator--(int)
    {
        return _val--;
    }

    template <typename T>
    inline IntegerIterator<T>& IntegerIterator<T>::
    operator+=(const typename IntegerIterator<T>::difference_type& diff)
    {
        _val += diff;
        return *this;
    }

    template <typename T>
    inline IntegerIterator<T>& IntegerIterator<T>::
    operator-=(const typename IntegerIterator<T>::difference_type& diff)
    {
        _val += diff;
        return *this;
    }

    template <typename T>
    inline IntegerIterator<T> IntegerIterator<T>::
    operator+(const typename IntegerIterator<T>::difference_type& diff) const
    {
        return IntegerIterator<T>(_val + diff);
    }

    template <typename T>
    inline IntegerIterator<T> IntegerIterator<T>::
    operator-(const typename IntegerIterator<T>::difference_type& diff) const
    {
        return IntegerIterator<T>(_val - diff);
    }

    template <typename T>
    inline typename IntegerIterator<T>::difference_type IntegerIterator<T>::
    operator-(const IntegerIterator<T>& rhs) const
    {
        return _val - rhs._val;
    }

    template <typename T>
    inline bool IntegerIterator<T>::operator<(const IntegerIterator<T>& rhs) const
    {
        return _val < rhs._val;
    }

    template <typename T>
    inline bool IntegerIterator<T>::operator>(const IntegerIterator<T>& rhs) const
    {
        return _val > rhs._val;
    }

    template <typename T>
    inline bool IntegerIterator<T>::operator<=(const IntegerIterator<T>& rhs) const
    {
        return _val <= rhs._val;
    }

    template <typename T>
    inline bool IntegerIterator<T>::operator>=(const IntegerIterator<T>& rhs) const
    {
        return _val >= rhs._val;
    }

    template <typename T>
    inline bool IntegerIterator<T>::operator==(const IntegerIterator<T>& rhs) const
    {
        return _val == rhs._val;
    }

    template <typename T>
    inline bool IntegerIterator<T>::operator!=(const IntegerIterator<T>& rhs) const
    {
        return _val != rhs._val;
    }


    template <typename T>
    inline IntegerIterator<T> operator+(const IntegerIterator<T>& it,
                                        const typename IntegerIterator<T>::difference_type& diff)
    {
        return IntegerIterator<T>(diff + it._val);
    }

    template <typename T>
    inline IntegerIterator<T> operator-(const IntegerIterator<T>& it,
                                        const typename IntegerIterator<T>::difference_type& diff)
    {
        return IntegerIterator<T>(diff - it._val);
    }
}
