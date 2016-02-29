#ifndef HUBBARD_MODEL2D_HPP_
#define HUBBARD_MODEL2D_HPP_

#include <tuple>
#include <vector>
#include <ostream>

#include "hubbard/constants.hpp"

namespace hubbard
{
    namespace model2d
    {
        template <typename Real>
        struct Discretization {
            using RealType = Real;

            struct IndexType {
                std::size_t ix, iy;
                inline bool operator==(const IndexType& rhs) const
                {
                    return (ix == rhs.ix) && (iy == rhs.iy);
                };
                inline bool operator!=(const IndexType& rhs) const
                {
                    return (ix != rhs.ix) || (iy != rhs.iy);
                };
            };
            struct VectorType {
                Real x, y;
            };

            std::vector<IndexType> indices;
            std::vector<std::vector<VectorType>> sites;
            const std::size_t num_x, num_y;
            const Real dx, dy;
            const Real x_min, y_min, x_max, y_max;

            // init in real space
            Discretization(const std::size_t nx, const std::size_t ny, const Real& delta_x,
                           const Real& delta_y)
                : num_x(nx), num_y(ny), dx(delta_x), dy(delta_y), x_min(0.), y_min(0.),
                  x_max((nx - 1) * dx), y_max((ny - 1) * dy)
            {
                for(std::size_t i = 0; i < nx; ++i) {
                    sites.push_back(std::vector<VectorType>());
                    for(std::size_t j = 0; j < ny; ++j) {
                        sites[i].push_back({x_min + i * dx, y_min + j * dy});
                        indices.push_back({i, j});
                    }
                }
            }

            // init in momentum space
            Discretization(const std::size_t nx, const std::size_t ny)
                : num_x(nx), num_y(ny), dx(TwoPi<Real>::value / num_x),
                  dy(TwoPi<Real>::value / num_y), x_min(-Pi<Real>::value), y_min(-Pi<Real>::value),
                  x_max(Pi<Real>::value), y_max(Pi<Real>::value)
            {
                for(std::size_t i = 0; i < nx; ++i) {
                    sites.push_back(std::vector<VectorType>());
                    for(std::size_t j = 0; j < ny; ++j) {
                        sites[i].push_back({x_min + i * dx, y_min + j * dy});
                        indices.push_back({i, j});
                    }
                }
            }

            const VectorType& operator()(const IndexType& i) const
            {
                return sites[std::get<0>(i)][std::get<1>(i)];
            }
            VectorType& operator()(const IndexType& i)
            {
                return sites[std::get<0>(i)][std::get<1>(i)];
            }

            IndexType closest(const VectorType& v) const
            {
                IndexType current = {0, 0};
                Real current_dist = (v.x - sites[0][0].x) * (v.x - sites[0][0].x)
                                    + (v.y - sites[0][0].y) * (v.y - sites[0][0].y);
                Real dist;
                for(std::size_t i = 0; i < num_x; ++i) {
                    for(std::size_t j = 0; j < num_y; ++j) {
                        dist = (v.x - sites[i][j].x) * (v.x - sites[i][j].x)
                               + (v.y - sites[i][j].y) * (v.y - sites[i][j].y);
                        if(dist < current_dist) {
                            current_dist = dist;
                            current.ix   = i;
                            current.iy   = j;
                        }
                    }
                }
                return current;
            }
        };

        template<typename Real>
        std::ostream& operator<<(std::ostream& strm, const typename Discretization<Real>::IndexType& index)
        {
            strm << '(' << index.ix << ',' << index.iy << ')';
            return strm;
        }
    }
}

#endif
