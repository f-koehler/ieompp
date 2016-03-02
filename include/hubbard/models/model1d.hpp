#ifndef HUBBARD_MODELS_MODEL1D_HPP_
#define HUBBARD_MODELS_MODEL1D_HPP_

#include <vector>

#include "hubbard/constants.hpp"

namespace hubbard
{
    namespace model1d
    {
        template <typename Real>
        struct Discretization {
            using RealType   = Real;
            using IndexType  = std::size_t;
            using VectorType = Real;

            std::vector<IndexType> indices;
            std::vector<Real> sites;
            const std::size_t num_x;
            const Real dx;
            const Real x_min, x_max;

            Discretization(const std::size_t n, const Real& delta_x);
            Discretization(const std::size_t n);

            IndexType closest(const VectorType& v) const;

            inline const VectorType& operator()(const IndexType& i) const;
            inline VectorType& operator()(const IndexType& i);
        };
    }
}

#include "detail/model1d_impl.hpp"

#endif
