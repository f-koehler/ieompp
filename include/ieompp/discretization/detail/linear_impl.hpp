#include <cassert>
#include <cmath>

#include "ieompp/discretization/linear.hpp"

namespace ieompp
{
    namespace discretization
    {
        template <typename Real>
        LinearDiscretization<Real>::LinearDiscretization(const Index& num)
            : _num(num), _first(0), _last(num - 1), _x_min(-Pi<Real>::value),
              _x_max(Pi<Real>::value), _x_length(TwoPi<Real>::value), _dx(_x_length / _num),
              _lattice_vectors{{_dx}}
        {
            assert(num > 0);
        }

        template <typename Real>
        LinearDiscretization<Real>::LinearDiscretization(const Index& num, const Real& dx)
            : _num(num), _first(0), _last(num - 1), _x_min(0.), _x_max(dx * (_num - 1)),
              _x_length(num * dx), _dx(dx), _lattice_vectors{{_dx}}
        {
            assert(num > 0);
        }

        template <typename Real>
        std::array<typename LinearDiscretization<Real>::Index, 2>
        LinearDiscretization<Real>::neighbours(const Index& idx) const
        {
            return std::array<Index, 2>{
                {(idx > 0) ? idx - 1 : _last, (idx < _last) ? idx + 1 : _first}};
        }

        template <typename Real>
        std::array<typename LinearDiscretization<Real>::Index, 1>
        LinearDiscretization<Real>::unique_neighbours(const Index& idx) const
        {
            return std::array<Index, 1>{{(idx < _last) ? idx + 1 : _first}};
        }

        template <typename Real>
        typename LinearDiscretization<Real>::Index
        LinearDiscretization<Real>::closest(Vector v) const
        {
            const auto dx2 = _dx / 2;
            while(v < _x_min - dx2) v += _x_length;
            while(v > _x_max + dx2) v -= _x_length;
            Real min_dist = ((*this)[Index(0)] - v) * ((*this)[Index(0)] - v), dist;
            Index min = 0;
            for(auto idx : *this) {
                dist = ((*this)[idx] - v) * ((*this)[idx] - v);
                if(dist < min_dist) {
                    std::swap(dist, min_dist);
                    min = idx;
                }
            }
            return min;
        }

        template <typename Real>
        const typename LinearDiscretization<Real>::Index& LinearDiscretization<Real>::num() const
        {
            return _num;
        }

        template <typename Real>
        const typename LinearDiscretization<Real>::Index& LinearDiscretization<Real>::first() const
        {
            return _first;
        }

        template <typename Real>
        const typename LinearDiscretization<Real>::Index& LinearDiscretization<Real>::last() const
        {
            return _last;
        }

        template <typename Real>
        const Real& LinearDiscretization<Real>::x_min() const
        {
            return _x_min;
        }

        template <typename Real>
        const Real& LinearDiscretization<Real>::x_max() const
        {
            return _x_max;
        }

        template <typename Real>
        const Real& LinearDiscretization<Real>::x_length() const
        {
            return _x_length;
        }

        template <typename Real>
        const Real& LinearDiscretization<Real>::dx() const
        {
            return _dx;
        }

        template <typename Real>
        const std::array<typename LinearDiscretization<Real>::Vector, 1>&
        LinearDiscretization<Real>::lattice_vectors() const
        {
            return _lattice_vectors;
        }

        template <typename Real>
        typename LinearDiscretization<Real>::ConstIndexIterator
        LinearDiscretization<Real>::begin() const
        {
            return ConstIndexIterator(_first);
        };

        template <typename Real>
        typename LinearDiscretization<Real>::ConstIndexIterator
        LinearDiscretization<Real>::end() const
        {
            return ConstIndexIterator(_last + 1);
        };

        template <typename Real>
        typename LinearDiscretization<Real>::IndexIterator LinearDiscretization<Real>::begin()
        {
            return IndexIterator(_first);
        };

        template <typename Real>
        typename LinearDiscretization<Real>::IndexIterator LinearDiscretization<Real>::end()
        {
            return IndexIterator(_last + 1);
        };

        template <typename Real>
        typename LinearDiscretization<Real>::ConstIndexIterator
        LinearDiscretization<Real>::cbegin() const
        {
            return ConstIndexIterator(_first);
        };

        template <typename Real>
        typename LinearDiscretization<Real>::ConstIndexIterator
        LinearDiscretization<Real>::cend() const
        {
            return ConstIndexIterator(_last + 1);
        };

        template <typename Real>
        typename LinearDiscretization<Real>::Vector LinearDiscretization<Real>::
        operator[](const Index& i) const
        {
            return _x_min + i * _dx;
        }

        template <typename Real>
        typename LinearDiscretization<Real>::Index LinearDiscretization<Real>::
        operator[](Vector v) const
        {
            while(v < _x_min) v += _x_length;
            while(v > _x_max) v -= _x_length;
            return Index(std::round((v - _x_min) / _dx));
        }
    }
}
