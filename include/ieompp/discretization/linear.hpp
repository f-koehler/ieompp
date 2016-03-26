#ifndef IEOMPP_DISCRETIZATION_LINEAR_HPP_
#define IEOMPP_DISCRETIZATION_LINEAR_HPP_

#include <vector>
#include <array>

#include "ieompp/constants.hpp"
#include "ieompp/iterators/integer_iterator.hpp"

namespace ieompp
{
    namespace discretization
    {
        template <typename RealT, typename IndexT>
        class LinearDiscretization {
            public:
                using Real               = RealT;
                using Index              = IndexT;
                using IndexIterator      = iterators::IntegerIterator<Index, false>;
                using ConstIndexIterator = iterators::IntegerIterator<Index, true>;
                using Vector             = Real;

            private:
                const Index _first, _last;
                const Index _num;
                const Real _x_min, _x_max;
                const Real _x_length, _dx;
                const std::array<Vector, 2> _lattice_vectors;

            public:
                LinearDiscretization(const Index& first, const Index& past_end);
                LinearDiscretization(const Index& first, const Index& past_end, const Real& dx);
                LinearDiscretization(const Index& first, const Index& past_end, const Real& x_min,
                                     const Real& x_max);

                std::array<Index, 2> neighbours(const Index& idx) const;
                std::array<Index, 1> unique_neighbours(const Index& idx) const;

                const Index& num() const;
                const std::array<Vector, 2>& lattice_vectors() const;

                ConstIndexIterator begin() const;
                ConstIndexIterator end() const;
                IndexIterator begin();
                IndexIterator end();
                ConstIndexIterator cbegin() const;
                ConstIndexIterator cend() const;

                const Vector operator[](const Index& i) const;
                const Index operator[](Vector v) const;
        };
    }
}

#include "detail/linear_impl.hpp"

#endif
