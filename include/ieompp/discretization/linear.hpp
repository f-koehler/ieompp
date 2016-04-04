#ifndef IEOMPP_DISCRETIZATION_LINEAR_HPP_
#define IEOMPP_DISCRETIZATION_LINEAR_HPP_

#include <array>
#include <tuple>
#include <vector>

#include "ieompp/constants.hpp"
#include "ieompp/iterators/integer_iterator.hpp"

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
    }
}

#include "detail/linear_impl.hpp"

#endif
