#ifndef IEOMPP_ODE_RKCK_HPP_
#define IEOMPP_ODE_RKCK_HPP_

#include <cassert>

#include <ieompp/description.hpp>
#include <ieompp/ode/step_size_controller.hpp>
#include <ieompp/types/matrix_check.hpp>

namespace ieompp
{
    namespace ode
    {
        template <typename FloatT>
        class RKCK
        {
            using Float = FloatT;

        private:
            const std::size_t _dimension;
            Float _step_size;
            Float _last_error;
            StepSizeController<Float> _controller;

        public:
            RKCK(std::size_t dimension, const Float& step_size)
                : _dimension(dimension), _step_size(step_size), _last_error(0.)
            {
            }

            const Float& step_size() const { return _step_size; }
            Float& step_size() { return _step_size; }

            std::size_t dimension() const { return _dimension; }
            const Float& last_error() const { return _last_error; }

            const Float& controller() const { return _controller; }
            Float& controller() { return _controller; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u)
            {
                assert(types::is_quadratic(m));
                assert(m.rows() == _dimension);
                assert(u.cols() == 1);
                assert(u.rows() == _dimension);

                static const Float a_2_1 = Float(1) / Float(5);
                static const Float a_3_1 = Float(3) / Float(40), a_3_2 = Float(9) / Float(40);
                static const Float a_4_1 = Float(3) / Float(10), a_4_2 = Float(-9) / Float(10),
                                  a_4_3 = Float(6) / Float(5);
                static const Float a_5_1 = Float(-11) / Float(54), a_5_2 = Float(5) / Float(2),
                                  a_5_3 = Float(-70) / Float(27), a_5_4 = Float(35) / Float(27);
                static const Float a_6_1 = Float(1631) / Float(55296), a_6_2 = Float(175) / Float(512),
                                  a_6_3 = Float(575) / Float(13824),
                                  a_6_4 = Float(44275) / Float(110592),
                                  a_6_5 = Float(253) / Float(4096);
                static const Float b4_1 = Float(2825) / Float(27648), b4_2 = Float(0),
                                  b4_3 = Float(18575) / Float(48384),
                                  b4_4 = Float(13525) / Float(55296), b4_5 = Float(277) / Float(14336),
                                  b4_6 = Float(1) / Float(4);
                static const Float b5_1 = Float(37) / Float(378), b5_2 = Float(0),
                                  b5_3 = Float(250) / Float(621), b5_4 = Float(125) / Float(594),
                                  b5_5 = Float(0), b5_6 = Float(512) / Float(1771);

                static Vector k_1, k_2, k_3, k_4, k_5, k_6;
                static Vector u4, u5;

                k_1 = m * u;
                do {
                    k_2 = m * (u + _step_size * a_2_1 * k_1);
                    k_3 = m * (u + _step_size * (a_3_1 * k_1 + a_3_2 * k_2));
                    k_4 = m * (u + _step_size * (a_4_1 * k_1 + a_4_2 * k_2 + a_4_3 * k_3));
                    k_5 = m * (u
                               + _step_size
                                     * (a_5_1 * k_1 + a_5_2 * k_2 + a_5_3 * k_3 + a_5_4 * k_4));
                    k_6 = m * (u
                               + _step_size * (a_6_1 * k_1 + a_6_2 * k_2 + a_6_3 * k_3 + a_6_4 * k_4
                                               + a_6_5 * k_5));

                    u4 = u
                         + _step_size
                               * (b4_1 * k_1 + b4_3 * k_3 + b4_4 * k_4 + b4_5 * k_5 + b4_6 * k_6);
                    u5 = u
                         + _step_size
                               * (b5_1 * k_1 + b5_3 * k_3 + b5_4 * k_4 + b5_5 * k_5 + b5_6 * k_6);

                    _last_error = (u4 - u5).norm();
                    if(!_controller(_step_size, _last_error)) {
                        u = u5;
                        break;
                    }
                } while(true);
            }
        };
    }
}

#endif
