#ifndef IEOMPP_DISCRETIZATION_SQUARE_HPP_
#define IEOMPP_DISCRETIZATION_SQUARE_HPP_

#include <array>
#include <cstddef>
#include <type_traits>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Dense>
#pragma GCC diagnostic pop

#include "ieompp/iterators/integer_iterator.hpp"

namespace ieompp
{
    namespace discretization
    {
        template <typename RealT, typename IndexT = std::size_t>
        class SquareDiscretization {
            public:
                using Real               = RealT;
                using Index              = IndexT;
                using IndexIterator      = iterators::IntegerIterator<Index, false>;
                using ConstIndexIterator = iterators::IntegerIterator<Index, true>;
                using Vector             = Eigen::Matrix<Real, 2, 1>;

            private:
                const Index _num_x, _num_y, _num;
                const Index _first, _last;
                const Real _x_min, _x_max;
                const Real _x_length, _dx;
                const Real _y_min, _y_max;
                const Real _y_length, _dy;
                const std::array<Vector, 2> _lattice_vectors;

            public:
                SquareDiscretization(const Index& num_x, const Index& num_y);
                SquareDiscretization(const Index& num_x, const Index& num_y, const Real& dx,
                                     const Real& dy);

                Index index(const Index& i, const Index& j) const;

                std::array<Index, 4> neighbours(const Index& idx) const;
                std::array<Index, 2> unique_neighbours(const Index& idx) const;
                Index closest(Vector v) const;

                const Index& num() const;
                const Index& first() const;
                const Index& last() const;
                const Real& x_min() const;
                const Real& x_max() const;
                const Real& x_length() const;
                const Real& dx() const;
                const Real& y_min() const;
                const Real& y_max() const;
                const Real& y_length() const;
                const Real& dy() const;
                const std::array<Vector, 2>& lattice_vectors() const;

                ConstIndexIterator begin() const;
                ConstIndexIterator end() const;
                IndexIterator begin();
                IndexIterator end();
                ConstIndexIterator cbegin() const;
                ConstIndexIterator cend() const;

                template <typename IndexT_>
                typename std::enable_if<std::is_same<Index, IndexT_>::value, Vector>::type
                operator[](const IndexT_& i) const;

                Index operator()(Vector v) const;
        };
    }
}

#include "detail/square_impl.hpp"

#endif
