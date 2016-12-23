#ifndef IEOMPP_LATTICES_PERIODIC_SQUARE_LATTICE_HPP_
#define IEOMPP_LATTICES_PERIODIC_SQUARE_LATTICE_HPP_

#include "ieompp/constants.hpp"
#include "ieompp/iterators/integer_iterator.hpp"
#include "ieompp/types/blaze.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <type_traits>

namespace ieompp
{
    namespace lattices
    {
        template <typename FloatT, typename IndexT = uint64_t>
        class PeriodicSquareLattice
        {
        public:
            using Float              = FloatT;
            using SiteIndex          = IndexT;
            using IndexIterator      = iterators::IntegerIterator<SiteIndex, false>;
            using ConstIndexIterator = iterators::IntegerIterator<SiteIndex, true>;
            using Vector             = blaze::StaticVector<Float, 2ul, blaze::columnVector>;

            static constexpr uint64_t coordination_number = 4;

        private:
            const SiteIndex _size_x, _size_y, _size;
            const SiteIndex _first, _last;
            const Float _x_min, _x_max;
            const Float _x_length, _dx;
            const Float _y_min, _y_max;
            const Float _y_length, _dy;
            const std::array<Vector, 2> _lattice_vectors;

        public:
            PeriodicSquareLattice(const SiteIndex& size_x, const SiteIndex& size_y);
            PeriodicSquareLattice(const SiteIndex& size_x, const SiteIndex& size_y, const Float& dx,
                                  const Float& dy);

            SiteIndex index(const SiteIndex& i, const SiteIndex& j) const;
            SiteIndex x_index(const SiteIndex& i) const;
            SiteIndex y_index(const SiteIndex& i) const;

            std::array<SiteIndex, 4> neighbors(const SiteIndex& idx) const;
            std::array<SiteIndex, 2> unique_neighbors(const SiteIndex& idx) const;
            SiteIndex closest(Vector v) const;

            const SiteIndex& size() const;
            const SiteIndex& size_x() const;
            const SiteIndex& size_y() const;
            const SiteIndex& first() const;
            const SiteIndex& last() const;
            const Float& x_min() const;
            const Float& x_max() const;
            const Float& x_length() const;
            const Float& dx() const;
            const Float& y_min() const;
            const Float& y_max() const;
            const Float& y_length() const;
            const Float& dy() const;
            const std::array<Vector, 2>& lattice_vectors() const;

            SiteIndex lattice_distance_x(const SiteIndex& a, const SiteIndex& b) const;
            SiteIndex lattice_distance_y(const SiteIndex& a, const SiteIndex& b) const;

            bool neighboring(const SiteIndex a, const SiteIndex b) const;

            ConstIndexIterator begin() const;
            ConstIndexIterator end() const;
            IndexIterator begin();
            IndexIterator end();
            ConstIndexIterator cbegin() const;
            ConstIndexIterator cend() const;

            template <typename IndexT_>
            typename std::enable_if<std::is_same<SiteIndex, IndexT_>::value, Vector>::type
            operator[](const IndexT_& i) const;

            SiteIndex operator()(Vector v) const;
        };


        template <typename Float, typename Index>
        PeriodicSquareLattice<Float, Index>::PeriodicSquareLattice(const Index& Nx, const Index& Ny)
            : _size_x(Nx), _size_y(Ny), _size(Nx * Ny), _first(0), _last(_size - 1),
              _x_min(-Pi<Float>::value), _x_max(Pi<Float>::value), _x_length(TwoPi<Float>::value),
              _dx(TwoPi<Float>::value / Nx), _y_min(-Pi<Float>::value), _y_max(Pi<Float>::value),
              _y_length(TwoPi<Float>::value), _dy(TwoPi<Float>::value / Ny),
              _lattice_vectors{{Vector{_dx, 0.}, Vector{0., _dy}}}
        {
            assert(_size_x > 0);
            assert(_size_y > 0);
            assert(_size > 0);
        }

        template <typename Float, typename Index>
        PeriodicSquareLattice<Float, Index>::PeriodicSquareLattice(const Index& Nx, const Index& Ny,
                                                                   const Float& dx, const Float& dy)
            : _size_x(Nx), _size_y(Ny), _size(Nx * Ny), _first(0), _last(_size - 1), _x_min(0.),
              _x_max((Nx - 1) * dx), _x_length(_size_x * dx), _dx(dx), _y_min(0.),
              _y_max((Ny - 1) * dy), _y_length(_size_y * dy), _dy(dy),
              _lattice_vectors{{Vector{_dx, 0.}, Vector{0., _dy}}}
        {
            assert(_size_x > 0);
            assert(_size_y > 0);
            assert(_size > 0);
        }

        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::index(const Index& i, const Index& j) const
        {
            return i * _size_x + j;
        }

        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::x_index(const Index& i) const
        {
            return i % _size_x;
        }

        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::y_index(const Index& i) const
        {
            return i / _size_x;
        }

        template <typename Float, typename Index>
        std::array<Index, 4> PeriodicSquareLattice<Float, Index>::neighbors(const Index& idx) const
        {
            const auto i = idx / _size_y;
            const auto j = idx % _size_x;
            return std::array<Index, 4>{{index((i == 0) ? _size_x - 1 : i - 1, j),
                                         index(i, (j == 0) ? _size_y - 1 : j - 1),
                                         index((i == _size_x - 1) ? 0 : i + 1, j),
                                         index(i, (j == _size_y - 1) ? 0 : j + 1)}};
        }

        template <typename Float, typename Index>
        std::array<Index, 2>
        PeriodicSquareLattice<Float, Index>::unique_neighbors(const Index& idx) const
        {
            const auto i = idx / _size_y;
            const auto j = idx % _size_x;
            return std::array<Index, 2>{{index((i == _size_x - 1) ? 0 : i + 1, j),
                                         index(i, (j == _size_y - 1) ? 0 : j + 1)}};
        }

        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::closest(Vector v) const
        {
            const auto dx2 = _dx / 2;
            const auto dy2 = _dy / 2;
            while(v[0] < _x_min - dx2) v[0] += _x_length;
            while(v[0] > _x_max + dx2) v[0] -= _x_length;
            while(v[1] < _y_min - dy2) v[1] += _y_length;
            while(v[1] > _y_max + dy2) v[1] -= _y_length;
            Float min_dist = types::dot_product(v - (*this)[Index(0)], v - (*this)[Index(0)]);
            Float dist;
            Index min = 0;
            for(auto idx : *this) {
                dist = types::dot_product(v - (*this)[idx], v - (*this)[idx]);
                if(dist < min_dist) {
                    std::swap(min_dist, dist);
                    min = idx;
                }
            }
            return min;
        }

        template <typename Float, typename Index>
        const Index& PeriodicSquareLattice<Float, Index>::size() const
        {
            return _size;
        }

        template <typename Float, typename Index>
        const Index& PeriodicSquareLattice<Float, Index>::size_x() const
        {
            return _size_x;
        }

        template <typename Float, typename Index>
        const Index& PeriodicSquareLattice<Float, Index>::size_y() const
        {
            return _size_y;
        }

        template <typename Float, typename Index>
        const Index& PeriodicSquareLattice<Float, Index>::first() const
        {
            return _first;
        }

        template <typename Float, typename Index>
        const Index& PeriodicSquareLattice<Float, Index>::last() const
        {
            return _last;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::x_min() const
        {
            return _x_min;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::x_max() const
        {
            return _x_max;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::x_length() const
        {
            return _x_length;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::dx() const
        {
            return _dx;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::y_min() const
        {
            return _y_min;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::y_max() const
        {
            return _y_max;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::y_length() const
        {
            return _y_length;
        }

        template <typename Float, typename Index>
        const Float& PeriodicSquareLattice<Float, Index>::dy() const
        {
            return _dy;
        }

        template <typename Float, typename Index>
        const std::array<typename PeriodicSquareLattice<Float, Index>::Vector, 2>&
        PeriodicSquareLattice<Float, Index>::lattice_vectors() const
        {
            return _lattice_vectors;
        }


        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::lattice_distance_x(const Index& a,
                                                                      const Index& b) const
        {
            const auto a_x  = x_index(a);
            const auto b_x  = x_index(b);
            const auto dist = (a_x > b_x) ? a_x - b_x : b_x - a_x;
            return std::min(dist, _size_x - dist);
        }

        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::lattice_distance_y(const Index& a,
                                                                      const Index& b) const
        {
            const auto a_y  = y_index(a);
            const auto b_y  = y_index(b);
            const auto dist = (a_y > b_y) ? a_y - b_y : b_y - a_y;
            return std::min(dist, _size_y - dist);
        }

        template <typename Float, typename Index>
        bool PeriodicSquareLattice<Float, Index>::neighboring(const Index a, const Index b) const
        {
            const auto x_a = a / _size_y;
            const auto y_a = a % _size_x;
            const auto x_b = b / _size_y;
            const auto y_b = b % _size_x;

            if(x_a == x_b) {
                return ((y_a + 1) % _size_y == y_b) || ((y_b + 1) % _size_y == y_a);
            }

            if(y_a == y_b) {
                return ((x_a + 1) % _size_x == x_b) || ((x_b + 1) % _size_x == x_a);
            }

            return false;
        }

        template <typename Float, typename Index>
        typename PeriodicSquareLattice<Float, Index>::ConstIndexIterator
        PeriodicSquareLattice<Float, Index>::begin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Float, typename Index>
        typename PeriodicSquareLattice<Float, Index>::ConstIndexIterator
        PeriodicSquareLattice<Float, Index>::end() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Float, typename Index>
        typename PeriodicSquareLattice<Float, Index>::IndexIterator
        PeriodicSquareLattice<Float, Index>::begin()
        {
            return IndexIterator(_first);
        }

        template <typename Float, typename Index>
        typename PeriodicSquareLattice<Float, Index>::IndexIterator
        PeriodicSquareLattice<Float, Index>::end()
        {
            return IndexIterator(_last + 1);
        }

        template <typename Float, typename Index>
        typename PeriodicSquareLattice<Float, Index>::ConstIndexIterator
        PeriodicSquareLattice<Float, Index>::cbegin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Float, typename Index>
        typename PeriodicSquareLattice<Float, Index>::ConstIndexIterator
        PeriodicSquareLattice<Float, Index>::cend() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Float, typename Index>
        template <typename IndexT>
        typename std::enable_if<std::is_same<Index, IndexT>::value,
                                typename PeriodicSquareLattice<Float, Index>::Vector>::type
            PeriodicSquareLattice<Float, Index>::operator[](const IndexT& idx) const
        {
            const auto i = idx / _size_x;
            const auto j = idx % _size_x;
            return Vector{_x_min + i * _dx, _y_min + j * _dy};
        }

        template <typename Float, typename Index>
        Index PeriodicSquareLattice<Float, Index>::
        operator()(typename PeriodicSquareLattice<Float, Index>::Vector v) const
        {
            while(v[0] < _x_min) v[0] += _x_length;
            while(v[0] > _x_max) v[0] -= _x_length;
            while(v[1] < _y_min) v[1] += _y_length;
            while(v[1] > _y_max) v[1] -= _y_length;
            const auto i = Index(std::round((v[0] - _x_min) / _dx));
            const auto j = Index(std::round((v[1] - _y_min) / _dy));
            return i * _size_x + j;
        }
    }
}

#endif
