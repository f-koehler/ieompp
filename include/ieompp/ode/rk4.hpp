#ifndef IEOMPP_ODE_RK4_HPP_
#define IEOMPP_ODE_RK4_HPP_

#include <cassert>

#include <ieompp/description.hpp>
#include <ieompp/types/eigen/check.hpp>
#include <ieompp/types/multiply_assign.hpp>

namespace ieompp
{
    namespace ode
    {
        template <typename FloatT>
        class RK4
        {
            using Float = FloatT;

        private:
            const std::size_t _dimension;
            const Float _step_size;

        public:
            RK4(std::size_t dimension, const Float& step_size)
                : _dimension(dimension), _step_size(step_size)
            {
            }

            const Float& step_size() const { return _step_size; }
            std::size_t dimension() const { return _dimension; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u) const
            {
                assert(types::is_quadratic(m));
                assert(m.rows() == _dimension);
                assert(u.cols() == 1);
                assert(u.rows() == _dimension);

                static Vector k_1, k_2, k_3, k_4;

                k_1 = u;
                types::multiply_assign(m, k_1);

                k_2 = u + (_step_size / 2) * k_1;
                types::multiply_assign(m, k_2);

                k_3 = u + (_step_size / 2) * k_2;
                types::multiply_assign(m, k_3);

                k_4 = u + _step_size * k_3;
                types::multiply_assign(m, k_4);

                u += (_step_size / 6) * (k_1 + 2. * k_2 + 2. * k_3 + k_4);
            }
        };
    }

    template <typename Float>
    struct TypeDescription<ode::RK4<Float>> {
        static const Description description;
    };

    template <typename Float>
    const Description TypeDescription<ode::RK4<Float>>::description =
        Description{{"type", "RK4 integrator"}, {"real type", ""}}
        + get_type_description<Float>().indent(2);
}

#endif
