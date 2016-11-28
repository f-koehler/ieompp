#ifndef IEOMPP_LATICES_LINEAR_HPP_
#define IEOMPP_LATICES_LINEAR_HPP_

#include "ieompp/constants.hpp"
#include "ieompp/description.hpp"
#include "ieompp/iterators/integer_iterator.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <type_traits>

namespace ieompp
{
    namespace lattices
    {
        template <typename FloatT, typename SiteIndexT = uint64_t>
        class LinearDiscretization
        {
        public:
            using Float         = FloatT;
            using SiteIndex     = SiteIndexT;
            using Iterator      = iterators::IntegerIterator<SiteIndex, false>;
            using ConstIterator = iterators::IntegerIterator<SiteIndex, true>;
            using Vector        = Float;

            static constexpr uint64_t coordination_number = 2;

        private:
            const SiteIndex _size;
            const SiteIndex _first, _last;
            const Float _x_min, _x_max;
            const Float _x_length, _dx;
            const std::array<Vector, 1> _lattice_vectors;

        public:
            LinearDiscretization(const SiteIndex& N);
            LinearDiscretization(const SiteIndex& N, const Float& dx);

            std::array<SiteIndex, 2> neighbors(const SiteIndex& idx) const;
            std::array<SiteIndex, 1> unique_neighbors(const SiteIndex& idx) const;
            SiteIndex closest(Vector v) const;

            const SiteIndex& size() const;
            const SiteIndex& first() const;
            const SiteIndex& last() const;
            const Float& x_min() const;
            const Float& x_max() const;
            const Float& x_length() const;
            const Float& dx() const;
            const std::array<Vector, 1>& lattice_vectors() const;

            template <uint64_t axis = 0>
            SiteIndex lattice_distance(const SiteIndex& a, const SiteIndex& b) const;

            bool neighboring(SiteIndex a, SiteIndex b) const;

            ConstIterator begin() const;
            ConstIterator end() const;
            Iterator begin();
            Iterator end();
            ConstIterator cbegin() const;
            ConstIterator cend() const;

            template <typename SiteIndex_>
            typename std::enable_if<std::is_same<SiteIndex, SiteIndex_>::value, Vector>::type
            operator[](const SiteIndex_& i) const;

            template <typename Vector_>
            typename std::enable_if<std::is_same<Vector, Vector_>::value, SiteIndex>::type
            operator()(Vector_ v) const;
        };


        template <typename Float, typename SiteIndex>
        LinearDiscretization<Float, SiteIndex>::LinearDiscretization(const SiteIndex& N)
            : _size(N), _first(0), _last(N - 1), _x_min(-Pi<Float>::value),
              _x_max(Pi<Float>::value), _x_length(TwoPi<Float>::value), _dx(_x_length / _size),
              _lattice_vectors{{_dx}}
        {
            assert(N > 0);
        }

        template <typename Float, typename SiteIndex>
        LinearDiscretization<Float, SiteIndex>::LinearDiscretization(const SiteIndex& N,
                                                                     const Float& dx)
            : _size(N), _first(0), _last(N - 1), _x_min(0.), _x_max(dx * (_size - 1)),
              _x_length(N * dx), _dx(dx), _lattice_vectors{{_dx}}
        {
            assert(N > 0);
        }

        template <typename Float, typename SiteIndex>
        std::array<SiteIndex, 2>
        LinearDiscretization<Float, SiteIndex>::neighbors(const SiteIndex& idx) const
        {
            return std::array<SiteIndex, 2>{
                {(idx > 0) ? idx - 1 : _last, (idx < _last) ? idx + 1 : _first}};
        }

        template <typename Float, typename SiteIndex>
        std::array<SiteIndex, 1>
        LinearDiscretization<Float, SiteIndex>::unique_neighbors(const SiteIndex& idx) const
        {
            return std::array<SiteIndex, 1>{{(idx < _last) ? idx + 1 : _first}};
        }

        template <typename Float, typename SiteIndex>
        SiteIndex LinearDiscretization<Float, SiteIndex>::closest(Vector v) const
        {
            const auto dx2 = _dx / 2;
            while(v < _x_min - dx2) {
                v += _x_length;
            }
            while(v > _x_max + dx2) {
                v -= _x_length;
            }
            Float min_dist = ((*this)[SiteIndex(0)] - v) * ((*this)[SiteIndex(0)] - v), dist;
            SiteIndex min  = 0;
            for(auto idx : *this) {
                dist = ((*this)[idx] - v) * ((*this)[idx] - v);
                if(dist < min_dist) {
                    std::swap(dist, min_dist);
                    min = idx;
                }
            }
            return min;
        }

        template <typename Float, typename SiteIndex>
        const SiteIndex& LinearDiscretization<Float, SiteIndex>::size() const
        {
            return _size;
        }

        template <typename Float, typename SiteIndex>
        const SiteIndex& LinearDiscretization<Float, SiteIndex>::first() const
        {
            return _first;
        }

        template <typename Float, typename SiteIndex>
        const SiteIndex& LinearDiscretization<Float, SiteIndex>::last() const
        {
            return _last;
        }

        template <typename Float, typename SiteIndex>
        const Float& LinearDiscretization<Float, SiteIndex>::x_min() const
        {
            return _x_min;
        }

        template <typename Float, typename SiteIndex>
        const Float& LinearDiscretization<Float, SiteIndex>::x_max() const
        {
            return _x_max;
        }

        template <typename Float, typename SiteIndex>
        const Float& LinearDiscretization<Float, SiteIndex>::x_length() const
        {
            return _x_length;
        }

        template <typename Float, typename SiteIndex>
        const Float& LinearDiscretization<Float, SiteIndex>::dx() const
        {
            return _dx;
        }

        template <typename Float, typename SiteIndex>
        const std::array<typename LinearDiscretization<Float, SiteIndex>::Vector, 1>&
        LinearDiscretization<Float, SiteIndex>::lattice_vectors() const
        {
            return _lattice_vectors;
        }

        template <typename Float, typename SiteIndex>
        template <uint64_t axis>
        SiteIndex LinearDiscretization<Float, SiteIndex>::lattice_distance(const SiteIndex& a,
                                                                           const SiteIndex& b) const
        {
            static const SiteIndex max_dist = _size / 2;
            SiteIndex dist                  = 0;
            if(b > a) {
                dist = b - a;
            } else {
                dist = a - b;
            }
            if(dist > max_dist) {
                return _size - dist;
            }
            return dist;
        }


        template <typename Float, typename SiteIndex>
        bool LinearDiscretization<Float, SiteIndex>::neighboring(const SiteIndex a,
                                                                 const SiteIndex b) const
        {
            return ((a + 1) % _size == b) || ((b + 1) % _size == a);
        }

        template <typename Float, typename SiteIndex>
        typename LinearDiscretization<Float, SiteIndex>::ConstIterator
        LinearDiscretization<Float, SiteIndex>::begin() const
        {
            return ConstIterator(_first);
        }

        template <typename Float, typename SiteIndex>
        typename LinearDiscretization<Float, SiteIndex>::ConstIterator
        LinearDiscretization<Float, SiteIndex>::end() const
        {
            return ConstIterator(_last + 1);
        }

        template <typename Float, typename SiteIndex>
        typename LinearDiscretization<Float, SiteIndex>::Iterator
        LinearDiscretization<Float, SiteIndex>::begin()
        {
            return Iterator(_first);
        }

        template <typename Float, typename SiteIndex>
        typename LinearDiscretization<Float, SiteIndex>::Iterator
        LinearDiscretization<Float, SiteIndex>::end()
        {
            return Iterator(_last + 1);
        }

        template <typename Float, typename SiteIndex>
        typename LinearDiscretization<Float, SiteIndex>::ConstIterator
        LinearDiscretization<Float, SiteIndex>::cbegin() const
        {
            return ConstIterator(_first);
        }

        template <typename Float, typename SiteIndex>
        typename LinearDiscretization<Float, SiteIndex>::ConstIterator
        LinearDiscretization<Float, SiteIndex>::cend() const
        {
            return ConstIterator(_last + 1);
        }

        template <typename Float, typename SiteIndex>
        template <typename SiteIndex_>
        typename std::enable_if<std::is_same<SiteIndex, SiteIndex_>::value,
                                typename LinearDiscretization<Float, SiteIndex>::Vector>::type
            LinearDiscretization<Float, SiteIndex>::operator[](const SiteIndex_& i) const
        {
            return _x_min + i * _dx;
        }

        template <typename Float, typename SiteIndex>
        template <typename Vector_>
        typename std::
            enable_if<std::is_same<typename LinearDiscretization<Float, SiteIndex>::Vector,
                                   Vector_>::value,
                      SiteIndex>::type
            LinearDiscretization<Float, SiteIndex>::operator()(Vector_ v) const
        {
            while(v < _x_min) {
                v += _x_length;
            }
            while(v > _x_max) {
                v -= _x_length;
            }
            return SiteIndex(std::round((v - _x_min) / _dx)) % _size;
        }
    } // namespace lattices
} // namespace ieompp

#endif
