#ifndef HUBBARD_MODELS_MODEL2D_HPP_
#define HUBBARD_MODELS_MODEL2D_HPP_

#include <tuple>
#include <vector>
#include <ostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Dense>
#pragma GCC diagnostic pop

#include "hubbard/constants.hpp"

namespace hubbard
{
    namespace model2d
    {
        template <typename Real>
        struct Discretization {
            using RealType = Real;
            using IndexType = std::tuple<std::size_t, std::size_t>;
            using VectorType = Eigen::Matrix<Real, 2, 1>;

            std::vector<IndexType> indices;
            std::vector<std::vector<VectorType>> sites;
            const std::size_t num_x, num_y;
            const Real dx, dy;
            const Real x_min, y_min, x_max, y_max;

            // init in real space
            Discretization(const std::size_t nx, const std::size_t ny, const Real& delta_x,
                           const Real& delta_y);

            // init in momentum space
            Discretization(const std::size_t nx, const std::size_t ny);

            IndexType closest(const VectorType& v) const;

            const VectorType& operator()(const IndexType& i) const;
            VectorType& operator()(const IndexType& i);
        };
    }
}

#include "detail/model2d_impl.hpp"

#endif
