#include <cmath>

#include "hubbard/discretization/linear.hpp"

namespace hubbard
{
    namespace discretization
    {
        template <typename Real>
        LinearDiscretization<Real>::LinearDiscretization(const std::size_t n, const Real& delta_x)
            : num_x(n), dx(delta_x), x_min(0.), x_max((n - 1) * dx), x_diff(x_max - x_min)
        {
            for(std::size_t i = 0; i < n; ++i) {
                indices.push_back(i);
                sites.push_back(x_min + delta_x * i);
            }
        }

        template <typename Real>
        LinearDiscretization<Real>::LinearDiscretization(const std::size_t n)
            : num_x(n), dx(TwoPi<Real>::value / num_x), x_min(-Pi<Real>::value),
              x_max(Pi<Real>::value), x_diff(x_max - x_min)
        {
            for(std::size_t i = 0; i < n; ++i) {
                indices.push_back(i);
                sites.push_back(x_min + dx * i);
            }
        }
        
        template<typename Real>
        inline bool LinearDiscretization<Real>::out_of_bounds(const Vector& v) const
        {
            return (v < x_min) || (v > x_max);
        }

        template <typename Real>
        const typename LinearDiscretization<Real>::Index&
        LinearDiscretization<Real>::closest(const Vector& v) const
        {
            const Index& current = indices.front();
            Vector diff          = v - sites[0];
            Real current_dist    = diff * diff;
            Real dist;
            for(std::size_t i = 1; i < num_x; ++i) {
                diff = v - sites[i];
                dist = diff * diff;
                if(dist < current_dist) {
                    current_dist = dist;
                    current      = indices[i];
                }
            }
            return current;
        }

        template <typename Real>
        typename LinearDiscretization<Real>::Index&
        LinearDiscretization<Real>::closest(const Vector& v)
        {
            Index& current = indices.front();
            Vector diff          = v - sites[0];
            Real current_dist    = diff * diff;
            Real dist;
            for(std::size_t i = 1; i < num_x; ++i) {
                diff = v - sites[i];
                dist = diff * diff;
                if(dist < current_dist) {
                    current_dist = dist;
                    current      = indices[i];
                }
            }
            return current;
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
        inline const typename LinearDiscretization<Real>::Vector& LinearDiscretization<Real>::
        operator[](const Index& i) const
        {
            return sites[i];
        }

        template <typename Real>
        inline typename LinearDiscretization<Real>::Vector& LinearDiscretization<Real>::
        operator[](const Index& i)
        {
            return sites[i];
        }

        template <typename Real>
        inline typename LinearDiscretization<Real>::Index& LinearDiscretization<Real>::
        operator[](const Vector& v)
        {
            return indices[std::size_t(std::round((v - x_min) / dx))];
        }

        template <typename Real>
        inline const typename LinearDiscretization<Real>::Index& LinearDiscretization<Real>::
        operator[](const Vector& v) const
        {
            return indices[std::size_t(std::round((v - x_min) / dx))];
        }

        template <typename RealT>
        inline typename LinearDiscretization<RealT>::Real
        LinearDiscretization<RealT>::dot_product(const Vector& a, const Vector& b)
        {
            return a * b;
        }
    }
}
