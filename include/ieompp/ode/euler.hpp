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
            Real _step_width;

        public:
            Euler(std::size_t dimension, const Real& step_width)
                : _dimension(dimension), _step_width(step_width)
            {
            }

            const Real& step_width() const { return _step_width; }
            std::size_t dimension() const { return _dimension; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u) const
            {
                u += _step_width * m * u;
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
