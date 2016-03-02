#include "hubbard/model2d/discretization.hpp"

namespace hubbard
{
    namespace model2d
    {
        template <typename Real>
        Discretization<Real>::Discretization(const std::size_t nx, const std::size_t ny,
                                             const Real& delta_x, const Real& delta_y)
            : num_x(nx), num_y(ny), dx(delta_x), dy(delta_y), x_min(0.), y_min(0.),
              x_max((nx - 1) * dx), y_max((ny - 1) * dy)
        {
            for(std::size_t i = 0; i < nx; ++i) {
                sites.push_back(std::vector<VectorType>());
                for(std::size_t j = 0; j < ny; ++j) {
                    sites[i].push_back({x_min + i * dx, y_min + j * dy});
                    indices.push_back(std::make_tuple(i, j));
                }
            }
        }

        // init in momentum space
        template <typename Real>
        Discretization<Real>::Discretization(const std::size_t nx, const std::size_t ny)
            : num_x(nx), num_y(ny), dx(TwoPi<Real>::value / num_x), dy(TwoPi<Real>::value / num_y),
              x_min(-Pi<Real>::value), y_min(-Pi<Real>::value), x_max(Pi<Real>::value),
              y_max(Pi<Real>::value)
        {
            for(std::size_t i = 0; i < nx; ++i) {
                sites.push_back(std::vector<VectorType>());
                for(std::size_t j = 0; j < ny; ++j) {
                    sites[i].push_back({x_min + i * dx, y_min + j * dy});
                    indices.push_back(std::make_tuple(i, j));
                }
            }
        }

        template <typename Real>
        typename Discretization<Real>::IndexType
        Discretization<Real>::closest(const VectorType& v) const
        {
            IndexType current = std::make_tuple(0, 0);
            VectorType diff   = v - sites[0][0];
            Real current_dist = diff * diff;
            Real dist;
            for(std::size_t i = 0; i < num_x; ++i) {
                for(std::size_t j = 0; j < num_y; ++j) {
                    diff = v - sites[0][0];
                    dist = diff * diff;
                    if(dist < current_dist) {
                        current_dist = dist;
                        current = std::make_tuple(i, j);
                    }
                }
            }
            return current;
        }

        template <typename Real>
        inline const typename Discretization<Real>::VectorType& Discretization<Real>::
        operator()(const IndexType& i) const
        {
            return sites[std::get<0>(i)][std::get<1>(i)];
        }

        template <typename Real>
        inline typename Discretization<Real>::VectorType& Discretization<Real>::
        operator()(const IndexType& i)
        {
            return sites[std::get<0>(i)][std::get<1>(i)];
        }
    }
}
