#ifndef IEOMPP_ODE_EULER_HPP_
#define IEOMPP_ODE_EULER_HPP_

#include "ieompp/description.hpp"
#include "ieompp/types/eigen/check.hpp"

#include <cassert>

namespace ieompp
{
    namespace ode
    {
        template <typename FloatT>
        class Euler
        {
            using Float = FloatT;

        private:
            const std::size_t _dimension;
            Float _step_size;

        public:
            Euler(std::size_t dimension, const Float& step_size)
                : _dimension(dimension), _step_size(step_size)
            {
            }

            const Float& step_size() const { return _step_size; }
            std::size_t dimension() const { return _dimension; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u) const
            {
                u += _step_size * m * u;
            }
        };
    }

    template <typename Float>
    struct TypeDescription<ode::Euler<Float>> {
        static Description get()
        {
            return {{"Euler Integrator", ""},
                    {"  fixed-step integrator", ""},
                    {"  real", compose(TypeProperties<Float>::name, ' ', "(size ",
                                       TypeProperties<Float>::size, ")")}};
        }
    };
}


#endif
