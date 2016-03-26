#include <cassert>
#include <cmath>

#include "ieompp/discretization/linear.hpp"

namespace ieompp
{
    namespace discretization
    {
        template <typename Real, typename Index>
        LinearDiscretization<Real, Index>::LinearDiscretization(const Index& first,
                                                                const Index& last)
            : _first(first), _last(last), _num(last - first + 1), _x_min(-Pi<Real>::value),
              _x_max(Pi<Real>::value), _x_length(_x_max - _x_min), _dx(_x_length / _num),
              _lattice_vectors{{_dx}}
        {
            assert(last >= first);
        }

        template <typename Real, typename Index>
        LinearDiscretization<Real, Index>::LinearDiscretization(const Index& first,
                                                                const Index& last, const Real& dx)
            : _first(first), _last(last), _num(last - first + 1), _x_min(0.),
              _x_max(dx * (_num - 1)), _x_length(_x_max - _x_min), _dx(dx), _lattice_vectors{{_dx}}
        {
            assert(last >= first);
        }

        template <typename Real, typename Index>
        LinearDiscretization<Real, Index>::LinearDiscretization(const Index& first,
                                                                const Index& last,
                                                                const Real& x_min,
                                                                const Real& x_max)
            : _first(first), _last(last), _num(last - first + 1), _x_min(x_min), _x_max(x_max),
              _x_length(_x_max - _x_min), _dx(_x_length / _num), _lattice_vectors(_dx)
        {
            assert(last >= first);
            assert(x_max > x_min);
        }

        template <typename Real, typename Index>
        std::array<typename LinearDiscretization<Real, Index>::Index, 2>
        LinearDiscretization<Real, Index>::neighbours(const Index& idx) const
        {
            return std::array<Index, 2>{
                {(idx > 0) ? idx - 1 : _last, (idx < _last) ? idx + 1 : _first}};
        }

        template <typename Real, typename Index>
        std::array<typename LinearDiscretization<Real, Index>::Index, 1>
        LinearDiscretization<Real, Index>::unique_neighbours(const Index& idx) const
        {
            return std::array<Index, 1>{{(idx < _last) ? idx + 1 : _first}};
        }

        template <typename Real, typename Index>
        const Index& LinearDiscretization<Real, Index>::num() const
        {
            return _num;
        }

        template <typename Real, typename Index>
        const std::array<typename LinearDiscretization<Real, Index>::Vector, 2>&
        LinearDiscretization<Real, Index>::lattice_vectors() const
        {
            return _lattice_vectors;
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::begin() const
        {
            return ConstIndexIterator(_first);
        };

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::end() const
        {
            return ConstIndexIterator(_last + 1);
        };

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::IndexIterator
        LinearDiscretization<Real, Index>::begin()
        {
            return IndexIterator(_first);
        };

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::IndexIterator
        LinearDiscretization<Real, Index>::end()
        {
            return IndexIterator(_last + 1);
        };

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::cbegin() const
        {
            return ConstIndexIterator(_first);
        };

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::cend() const
        {
            return ConstIndexIterator(_last + 1);
        };

        template <typename Real, typename Index>
        const typename LinearDiscretization<Real, Index>::Vector LinearDiscretization<Real, Index>::
        operator[](const Index& i) const
        {
            return _x_min + i * _dx;
        }

        template <typename Real, typename Index>
        const typename LinearDiscretization<Real, Index>::Index LinearDiscretization<Real, Index>::
        operator[](Vector v) const
        {
            while(v > _x_max) v -= _x_length;
            return Index(std::round((v - _x_min) / _dx));
        }
    }
}
