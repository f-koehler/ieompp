#ifndef IEOMPP_DISCRETIZATION_LINEAR_HPP_
#define IEOMPP_DISCRETIZATION_LINEAR_HPP_

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include <ieompp/constants.hpp>
#include <ieompp/description.hpp>
#include <ieompp/iterators/integer_iterator.hpp>
#include <ieompp/types/description.hpp>

namespace ieompp
{
    namespace discretization
    {
        template <typename FloatT, typename IndexT = uint64_t>
        class LinearDiscretization
        {
        public:
            using Float              = FloatT;
            using Index              = IndexT;
            using IndexIterator      = iterators::IntegerIterator<Index, false>;
            using ConstIndexIterator = iterators::IntegerIterator<Index, true>;
            using Vector             = Float;

            static constexpr uint64_t coordination_number = 2;

        private:
            const Index _num;
            const Index _first, _last;
            const Float _x_min, _x_max;
            const Float _x_length, _dx;
            const std::array<Vector, 1> _lattice_vectors;

        public:
            LinearDiscretization(const Index& num);
            LinearDiscretization(const Index& num, const Float& dx);

            std::array<Index, 2> neighbors(const Index& idx) const;
            std::array<Index, 1> unique_neighbors(const Index& idx) const;
            Index closest(Vector v) const;

            const Index& num() const;
            const Index& first() const;
            const Index& last() const;
            const Float& x_min() const;
            const Float& x_max() const;
            const Float& x_length() const;
            const Float& dx() const;
            const std::array<Vector, 1>& lattice_vectors() const;

            bool neighboring(const Index a, const Index b) const;

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


        template <typename Float, typename Index>
        LinearDiscretization<Float, Index>::LinearDiscretization(const Index& num)
            : _num(num), _first(0), _last(num - 1), _x_min(-Pi<Float>::value),
              _x_max(Pi<Float>::value), _x_length(TwoPi<Float>::value), _dx(_x_length / _num),
              _lattice_vectors{{_dx}}
        {
            assert(num > 0);
        }

        template <typename Float, typename Index>
        LinearDiscretization<Float, Index>::LinearDiscretization(const Index& num, const Float& dx)
            : _num(num), _first(0), _last(num - 1), _x_min(0.), _x_max(dx * (_num - 1)),
              _x_length(num * dx), _dx(dx), _lattice_vectors{{_dx}}
        {
            assert(num > 0);
        }

        template <typename Float, typename Index>
        std::array<typename LinearDiscretization<Float, Index>::Index, 2>
        LinearDiscretization<Float, Index>::neighbors(const Index& idx) const
        {
            return std::array<Index, 2>{
                {(idx > 0) ? idx - 1 : _last, (idx < _last) ? idx + 1 : _first}};
        }

        template <typename Float, typename Index>
        std::array<typename LinearDiscretization<Float, Index>::Index, 1>
        LinearDiscretization<Float, Index>::unique_neighbors(const Index& idx) const
        {
            return std::array<Index, 1>{{(idx < _last) ? idx + 1 : _first}};
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::Index
        LinearDiscretization<Float, Index>::closest(Vector v) const
        {
            const auto dx2 = _dx / 2;
            while(v < _x_min - dx2) {
                v += _x_length;
            }
            while(v > _x_max + dx2) {
                v -= _x_length;
            }
            Float min_dist = ((*this)[Index(0)] - v) * ((*this)[Index(0)] - v), dist;
            Index min      = 0;
            for(auto idx : *this) {
                dist = ((*this)[idx] - v) * ((*this)[idx] - v);
                if(dist < min_dist) {
                    std::swap(dist, min_dist);
                    min = idx;
                }
            }
            return min;
        }

        template <typename Float, typename Index>
        const typename LinearDiscretization<Float, Index>::Index&
        LinearDiscretization<Float, Index>::num() const
        {
            return _num;
        }

        template <typename Float, typename Index>
        const typename LinearDiscretization<Float, Index>::Index&
        LinearDiscretization<Float, Index>::first() const
        {
            return _first;
        }

        template <typename Float, typename Index>
        const typename LinearDiscretization<Float, Index>::Index&
        LinearDiscretization<Float, Index>::last() const
        {
            return _last;
        }

        template <typename Float, typename Index>
        const Float& LinearDiscretization<Float, Index>::x_min() const
        {
            return _x_min;
        }

        template <typename Float, typename Index>
        const Float& LinearDiscretization<Float, Index>::x_max() const
        {
            return _x_max;
        }

        template <typename Float, typename Index>
        const Float& LinearDiscretization<Float, Index>::x_length() const
        {
            return _x_length;
        }

        template <typename Float, typename Index>
        const Float& LinearDiscretization<Float, Index>::dx() const
        {
            return _dx;
        }

        template <typename Float, typename Index>
        const std::array<typename LinearDiscretization<Float, Index>::Vector, 1>&
        LinearDiscretization<Float, Index>::lattice_vectors() const
        {
            return _lattice_vectors;
        }


        template <typename Float, typename Index>
        bool LinearDiscretization<Float, Index>::neighboring(const Index a, const Index b) const
        {
            return ((a + 1) % _num == b) || ((b + 1) % _num == a);
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::ConstIndexIterator
        LinearDiscretization<Float, Index>::begin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::ConstIndexIterator
        LinearDiscretization<Float, Index>::end() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::IndexIterator
        LinearDiscretization<Float, Index>::begin()
        {
            return IndexIterator(_first);
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::IndexIterator
        LinearDiscretization<Float, Index>::end()
        {
            return IndexIterator(_last + 1);
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::ConstIndexIterator
        LinearDiscretization<Float, Index>::cbegin() const
        {
            return ConstIndexIterator(_first);
        }

        template <typename Float, typename Index>
        typename LinearDiscretization<Float, Index>::ConstIndexIterator
        LinearDiscretization<Float, Index>::cend() const
        {
            return ConstIndexIterator(_last + 1);
        }

        template <typename Float, typename Index>
        template <typename IndexT>
        typename std::enable_if<std::is_same<Index, IndexT>::value,
                                typename LinearDiscretization<Float, Index>::Vector>::type
            LinearDiscretization<Float, Index>::operator[](const IndexT& i) const
        {
            return _x_min + i * _dx;
        }

        template <typename Float, typename Index>
        template <typename VectorT>
        typename std::enable_if<std::is_same<typename LinearDiscretization<Float, Index>::Vector,
                                             VectorT>::value,
                                Index>::type
        LinearDiscretization<Float, Index>::operator()(VectorT v) const
        {
            while(v < _x_min) {
                v += _x_length;
            }
            while(v > _x_max) {
                v -= _x_length;
            }
            return Index(std::round((v - _x_min) / _dx)) % _num;
        }
    } // namespace discretization

    template <typename Float, typename Index>
    struct TypeDescription<discretization::LinearDiscretization<Float, Index>> {
        static Description description()
        {
            return Description{{"linear discretization", ""}, {"real type", ""}}
                   + get_type_description<Float>().indent(2) + Description{{"index type", ""}}
                   + get_type_description<Index>().indent(2);
        }
    };

    template <typename Float, typename Index>
    struct InstanceDescription<discretization::LinearDiscretization<Float, Index>> {
        static Description
        description(const discretization::LinearDiscretization<Float, Index>& disc)
        {
            return Description{{"number of sites", std::to_string(disc.num())},
                               {"dx", std::to_string(disc.dx())}};
            // TODO: be more verbose
        }
    };
} // namespace ieompp

#endif
