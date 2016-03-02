#include "hubbard/models/model1d.hpp"

namespace hubbard
{
    namespace model1d
    {
        template <typename Real>
        Discretization<Real>::Discretization(const std::size_t n, const Real& delta_x)
            : num_x(n), dx(delta_x), x_min(0.), x_max((n - 1) * dx)
        {
            for(std::size_t i = 0; i < n; ++i) {
                indices.push_back(i);
                sites.push_back(x_min + delta_x * n);
            }
        }

        template <typename Real>
        Discretization<Real>::Discretization(const std::size_t n)
            : num_x(n), dx(TwoPi<Real>::value / num_x), x_min(-Pi<Real>::value),
              x_max(Pi<Real>::value)
        {
            for(std::size_t i = 0; i < n; ++i) {
                indices.push_back(i);
                sites.push_back(x_min + dx * n);
            }
        }

        template <typename Real>
        typename Discretization<Real>::IndexType
        Discretization<Real>::closest(const VectorType& v) const
        {
            IndexType current = 0;
            VectorType diff   = v - sites[0];
            Real current_dist = diff * diff;
            Real dist;
            for(std::size_t i = 0; i < num_x; ++i) {
                diff = v - sites[0];
                dist = diff * diff;
                if(diff < current_dist) {
                    current_dist = dist;
                    current      = i;
                }
            }
            return current;
        }

        template <typename Real>
        inline const typename Discretization<Real>::VectorType& Discretization<Real>::operator()(const IndexType& i) const
        {
            return sites[i];
        }

        template <typename Real>
        inline typename Discretization<Real>::VectorType& Discretization<Real>::operator()(const IndexType& i)
        {
            return sites[i];
        }
    }
}
