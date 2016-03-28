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
        template <typename RealT>
        class LinearDiscretization {
            public:
                using Real               = RealT;
                using Index              = std::size_t;
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
                Index closest(const Vector& v) const;

                const Index& num() const;
                const std::array<Vector, 1>& lattice_vectors() const;

                ConstIndexIterator begin() const;
                ConstIndexIterator end() const;
                IndexIterator begin();
                IndexIterator end();
                ConstIndexIterator cbegin() const;
                ConstIndexIterator cend() const;

                Vector operator[](const Index& i) const;
                Index operator[](Vector v) const;
        };
    }
}

#include "detail/linear_impl.hpp"

#endif
