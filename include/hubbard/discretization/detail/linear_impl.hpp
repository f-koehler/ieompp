#include <cmath>

#include "hubbard/discretization/linear.hpp"

namespace hubbard
{
    namespace discretization
    {
        template <typename Real>
        std::vector<typename LinearDiscretization<Real>::Index>
        LinearDiscretization<Real>::init_indices()
        {
            std::vector<typename LinearDiscretization<Real>::Index> result;
            for(std::size_t i = 0; i < num; ++i) result.push_back(i);
            return result;
        }

        template <typename Real>
        std::vector<typename LinearDiscretization<Real>::Vector>
        LinearDiscretization<Real>::init_sites()
        {
            std::vector<Real> result;
            for(std::size_t i = 0; i < num; ++i) result.push_back(x_min + dx * i);
            return result;
        }

        template <typename Real>
        LinearDiscretization<Real>::LinearDiscretization(const std::size_t n, const Real& delta_x)
            : num(n), num_x(n), dx(delta_x), x_min(0.), x_max((n - 1) * dx), x_diff(x_max - x_min),
              lattice_vectors{{Vector(delta_x)}}, indices(init_indices()), sites(init_sites())
        {
        }

        template <typename Real>
        LinearDiscretization<Real>::LinearDiscretization(const std::size_t n)
            : num(n), num_x(n), dx(TwoPi<Real>::value / num_x), x_min(-Pi<Real>::value),
              x_max(Pi<Real>::value), x_diff(x_max - x_min),
              lattice_vectors{{Vector(TwoPi<Real>::value / num_x)}}, indices(init_indices()),
              sites(init_sites())
        {
        }
        
        template <typename Real>
        const typename LinearDiscretization<Real>::Index&
        LinearDiscretization<Real>::closest(const Vector& v) const
        {
            std::size_t current_pos = 0;
            Vector diff             = v - sites[0];
            Real current_dist       = diff * diff;
            Real dist;
            for(std::size_t i = 1; i < num_x; ++i) {
                diff = v - sites[i];
                dist = diff * diff;
                if(dist < current_dist) {
                    current_dist = dist;
                    current_pos = i;
                }
            }
            return indices[current_pos];
        }

        template <typename Real>
        inline std::array<typename LinearDiscretization<Real>::Index, 2>
        LinearDiscretization<Real>::neighbours(const Index& idx) const
        {
            return std::array<Index, 2>{
                {(idx == 0) ? num_x - 1 : idx - 1, (idx == num_x - 1) ? 0 : idx + 1}};
        }

        template <typename Real>
        inline std::array<typename LinearDiscretization<Real>::Index, 1>
        LinearDiscretization<Real>::unique_neighbours(const Index& idx) const
        {
            return std::array<Index, 1>{{(idx == num_x - 1) ? 0 : idx + 1}};
        }

        template <typename Real>
        inline typename std::vector<typename LinearDiscretization<Real>::Index>::const_iterator
        LinearDiscretization<Real>::begin() const
        {
            return indices.cbegin();
        }

        template <typename Real>
        inline typename std::vector<typename LinearDiscretization<Real>::Index>::const_iterator
        LinearDiscretization<Real>::end() const
        {
            return indices.cend();
        }

        template <typename Real>
        inline const typename LinearDiscretization<Real>::Vector& LinearDiscretization<Real>::
        operator[](const Index& i) const
        {
            return sites[i];
        }

        template <typename Real>
        inline const typename LinearDiscretization<Real>::Index& LinearDiscretization<Real>::
        operator[](const Vector& v) const
        {
            return indices[std::size_t(std::round((v - x_min) / dx))];
        }
    }
}
