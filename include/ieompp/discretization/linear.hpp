#ifndef IEOMPP_DISCRETIZATION_LINEAR_HPP_
#define IEOMPP_DISCRETIZATION_LINEAR_HPP_

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include <ieompp/constants.hpp>
#include <ieompp/iterators/integer_iterator.hpp>

namespace ieompp
{
    namespace discretization
    {
        template <typename RealT, typename IndexT = std::size_t>
        class LinearDiscretization {
            public:
                using Real               = RealT;
                using Index              = IndexT;
                using IndexIterator      = iterators::IntegerIterator<Index, false>;
                using ConstIndexIterator = iterators::IntegerIterator<Index, true>;
                using Vector             = Real;

            private:
                const Index _num;
                const Index _first, _last;
                const Real _x_min, _x_max;
                const Real _x_length, _dx;
                const std::array<Vector, 1> _lattice_vectors;

            public:
                LinearDiscretization(const Index& num);
                LinearDiscretization(const Index& num, const Real& dx);

                std::array<Index, 2> neighbours(const Index& idx) const;
                std::array<Index, 1> unique_neighbours(const Index& idx) const;
                Index closest(Vector v) const;

                const Index& num() const;
                const Index& first() const;
                const Index& last() const;
                const Real& x_min() const;
                const Real& x_max() const;
                const Real& x_length() const;
                const Real& dx() const;
                const std::array<Vector, 1>& lattice_vectors() const;

                ConstIndexIterator begin() const;
                ConstIndexIterator end() const;
                IndexIterator begin();
                IndexIterator end();
                ConstIndexIterator cbegin() const;
                ConstIndexIterator cend() const;

                template <typename IndexT_>
                typename std::enable_if<std::is_same<Index, IndexT_>::value, Vector>::type
                operator[](const IndexT_& i) const;

                template <typename VectorT>
                typename std::enable_if<std::is_same<Vector, VectorT>::value, Index>::type
                operator()(VectorT v) const;
        };


        template <typename Real, typename Index>
        LinearDiscretization<Real, Index>::LinearDiscretization(const Index& num)
            : _num(num), _first(0), _last(num - 1), _x_min(-Pi<Real>::value),
              _x_max(Pi<Real>::value), _x_length(TwoPi<Real>::value), _dx(_x_length / _num),
              _lattice_vectors{{_dx}}
        {
            assert(num > 0);
        }

        template <typename Real, typename Index>
        LinearDiscretization<Real, Index>::LinearDiscretization(const Index& num, const Real& dx)
            : _num(num), _first(0), _last(num - 1), _x_min(0.), _x_max(dx * (_num - 1)),
              _x_length(num * dx), _dx(dx), _lattice_vectors{{_dx}}
        {
            assert(num > 0);
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
        typename LinearDiscretization<Real, Index>::Index
        LinearDiscretization<Real, Index>::closest(Vector v) const
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

        template <typename Real, typename Index>
        const typename LinearDiscretization<Real, Index>::Index&
        LinearDiscretization<Real, Index>::num() const
        {
            return _num;
        }

        template <typename Real, typename Index>
        const typename LinearDiscretization<Real, Index>::Index&
        LinearDiscretization<Real, Index>::first() const
        {
            return _first;
        }

        template <typename Real, typename Index>
        const typename LinearDiscretization<Real, Index>::Index&
        LinearDiscretization<Real, Index>::last() const
        {
            return _last;
        }

        template <typename Real, typename Index>
        const Real& LinearDiscretization<Real, Index>::x_min() const
        {
            return _x_min;
        }

        template <typename Real, typename Index>
        const Real& LinearDiscretization<Real, Index>::x_max() const
        {
            return _x_max;
        }

        template <typename Real, typename Index>
        const Real& LinearDiscretization<Real, Index>::x_length() const
        {
            return _x_length;
        }

        template <typename Real, typename Index>
        const Real& LinearDiscretization<Real, Index>::dx() const
        {
            return _dx;
        }

        template <typename Real, typename Index>
        const std::array<typename LinearDiscretization<Real, Index>::Vector, 1>&
        LinearDiscretization<Real, Index>::lattice_vectors() const
        {
            return _lattice_vectors;
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::begin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::end() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::IndexIterator
        LinearDiscretization<Real, Index>::begin()
        {
            return IndexIterator(_first);
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::IndexIterator
        LinearDiscretization<Real, Index>::end()
        {
            return IndexIterator(_last + 1);
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::cbegin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Real, typename Index>
        typename LinearDiscretization<Real, Index>::ConstIndexIterator
        LinearDiscretization<Real, Index>::cend() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Real, typename Index>
        template <typename IndexT>
        typename std::enable_if<std::is_same<Index, IndexT>::value,
                                typename LinearDiscretization<Real, Index>::Vector>::type
            LinearDiscretization<Real, Index>::operator[](const IndexT& i) const
        {
            return _x_min + i * _dx;
        }

        template <typename Real, typename Index>
        template <typename VectorT>
        typename std::enable_if<std::is_same<typename LinearDiscretization<Real, Index>::Vector,
                                             VectorT>::value,
                                Index>::type
        LinearDiscretization<Real, Index>::operator()(VectorT v) const
        {
            while(v < _x_min) v += _x_length;
            while(v > _x_max) v -= _x_length;
            return Index(std::round((v - _x_min) / _dx)) % _num;
        }
    }
}

#endif
