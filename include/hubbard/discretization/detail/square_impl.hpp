#include "hubbard/discretization/square.hpp"

namespace hubbard
{
    namespace discretization
    {
        template <typename Real>
        std::vector<typename SquareDiscretization<Real>::Index>
        SquareDiscretization<Real>::init_indices()
        {
            std::vector<typename SquareDiscretization<Real>::Index> result;
            for(std::size_t i = 0; i < num_x; ++i) {
                for(std::size_t j = 0; j < num_y; ++j) result.push_back(std::make_tuple(i, j));
            }
            return result;
        }

        template <typename Real>
        std::vector<std::vector<typename SquareDiscretization<Real>::Vector>>
        SquareDiscretization<Real>::init_sites()
        {
            std::vector<std::vector<typename SquareDiscretization<Real>::Vector>> result;
            for(std::size_t i = 0; i < num_x; ++i) {
                result.push_back(std::vector<Vector>());
                for(std::size_t j = 0; j < num_y; ++j)
                    result[i].push_back({x_min + i * dx, y_min + j * dy});
            }
            return result;
        }

        template <typename Real>
        SquareDiscretization<Real>::SquareDiscretization(const std::size_t nx, const std::size_t ny,
                                                         const Real& delta_x, const Real& delta_y)
            : num(nx * ny), num_x(nx), num_y(ny), dx(delta_x), dy(delta_y), x_min(0.), y_min(0.),
              x_max((nx - 1) * dx), y_max((ny - 1) * dy), x_diff(x_max - x_min),
              y_diff(y_max - y_min), lattice_vectors{{Vector(dx, 0.), Vector(0., dy)}},
              indices(init_indices()), sites(init_sites())
        {
        }

        // init in momentum space
        template <typename Real>
        SquareDiscretization<Real>::SquareDiscretization(const std::size_t nx, const std::size_t ny)
            : num(nx * ny), num_x(nx), num_y(ny), dx(TwoPi<Real>::value / num_x),
              dy(TwoPi<Real>::value / num_y), x_min(-Pi<Real>::value), y_min(-Pi<Real>::value),
              x_max(Pi<Real>::value), y_max(Pi<Real>::value), x_diff(x_max - x_min),
              y_diff(y_max - y_min), lattice_vectors{{Vector(dx, 0.), Vector(0., dy)}},
              indices(init_indices()), sites(init_sites())
        {
        }

        template <typename Real>
        const typename SquareDiscretization<Real>::Index&
        SquareDiscretization<Real>::closest(const Vector& v) const
        {
            std::size_t current_pos = 0;
            Vector diff   = v - sites[0][0];
            Real current_dist = diff.dot(diff);
            Real dist;
            for(std::size_t i = 0; i < num_x; ++i) {
                for(std::size_t j = 0; j < num_y; ++j) {
                    diff = v - sites[i][j];
                    dist = diff.dot(diff);
                    if(dist < current_dist) {
                        current_dist = dist;
                        current_pos  = i * num_x + j;
                    }
                }
            }
            return indices[current_pos];
        }

        template <typename Real>
        typename std::array<typename SquareDiscretization<Real>::Index, 4>
        SquareDiscretization<Real>::neighbours(const Index& idx) const
        {
            auto ix = std::get<0>(idx);
            auto iy = std::get<1>(idx);
            return std::array<Index, 4>{{std::make_tuple((ix == 0) ? num_x - 1 : ix - 1, iy),
                                         std::make_tuple(ix, (iy == 0) ? num_y - 1 : iy - 1),
                                         std::make_tuple((ix == num_x - 1) ? 0 : ix + 1, iy),
                                         std::make_tuple(ix, (iy == num_y - 1) ? 0 : iy + 1)}};
        }

        template <typename Real>
        typename std::array<typename SquareDiscretization<Real>::Index, 2>
        SquareDiscretization<Real>::unique_neighbours(const Index& idx) const
        {
            auto ix = std::get<0>(idx);
            auto iy = std::get<1>(idx);
            return std::array<Index, 2>{{std::make_tuple((ix == num_x - 1) ? 0 : ix + 1, iy),
                                         std::make_tuple(ix, (iy == num_y - 1) ? 0 : iy + 1)}};
        }

        template <typename Real>
        typename SquareDiscretization<Real>::Vector
        SquareDiscretization<Real>::project(SquareDiscretization<Real>::Vector v) const
        {
            while(v[0] < x_min) v += x_diff;
            while(v[1] > x_max) v -= x_diff;
            while(v[2] < y_min) v += y_diff;
            while(v[3] > y_max) v -= y_diff;
        }

        template <typename Real>
        inline typename std::vector<typename SquareDiscretization<Real>::Index>::const_iterator
        SquareDiscretization<Real>::begin() const
        {
            return indices.cbegin();
        }

        template <typename Real>
        inline typename std::vector<typename SquareDiscretization<Real>::Index>::const_iterator
        SquareDiscretization<Real>::end() const
        {
            return indices.cend();
        }

        template <typename Real>
        inline const typename SquareDiscretization<Real>::Vector& SquareDiscretization<Real>::
        operator[](const Index& i) const
        {
            return sites[std::get<0>(i)][std::get<1>(i)];
        }

        template <typename Real>
        inline const typename SquareDiscretization<Real>::Index& SquareDiscretization<Real>::
        operator[](const Vector& v) const
        {
            auto ix = std::size_t(std::round(v[0] - x_min));
            auto iy = std::size_t(std::round(v[1] - y_min));
            return indices[ix * num_x + iy];
        }
    }
}
