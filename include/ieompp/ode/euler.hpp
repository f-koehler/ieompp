#ifndef IEOMPP_ODE_EULER_HPP_
#define IEOMPP_ODE_EULER_HPP_

#include <cassert>

#include <ieompp/description.hpp>
#include <ieompp/types/eigen/check.hpp>

namespace ieompp
{
    namespace ode
    {
        template <typename RealT>
        class Euler
        {
            using Real = RealT;

        private:
            const std::size_t _dimension;
            Real _step_size;

        public:
            Euler(std::size_t dimension, const Real& step_size)
                : _dimension(dimension), _step_size(step_size)
            {
            }

            const Real& step_size() const { return _step_size; }
            std::size_t dimension() const { return _dimension; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u) const
            {
                u += _step_size * m * u;
            }
        };
    }

    template <typename Real>
    struct TypeDescription<ode::Euler<Real>> {
        static Description get()
        {
            return {{"Euler Integrator", ""},
                    {"  fixed-step integrator", ""},
                    {"  real", compose(TypeProperties<Real>::name, ' ', "(size ",
                                       TypeProperties<Real>::size, ")")}};
        }
    };
}


#endif
