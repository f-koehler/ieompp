#ifndef IEOMPP_ODE_RKCK_HPP_
#define IEOMPP_ODE_RKCK_HPP_

#include <cassert>

#include <ieompp/description.hpp>
#include <ieompp/ode/step_size_controller.hpp>
#include <ieompp/types/eigen/check.hpp>

namespace ieompp
{
    namespace ode
    {
        template <typename RealT>
        class RKCK
        {
            using Real = RealT;

        private:
            const std::size_t _dimension;
            Real _step_size;
            Real _last_error;
            StepSizeController<Real> _controller;

        public:
            RKCK(std::size_t dimension, const Real& step_size)
                : _dimension(dimension), _step_size(step_size), _last_error(0.)
            {
            }

            const Real& step_size() const { return _step_size; }
            Real& step_size() { return _step_size; }

            std::size_t dimension() const { return _dimension; }
            const Real& last_error() const { return _last_error; }

            const Real& controller() const { return _controller; }
            Real& controller() { return _controller; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u)
            {
                assert(types::is_quadratic(m));
                assert(m.rows() == _dimension);
                assert(u.cols() == 1);
                assert(u.rows() == _dimension);

                static const Real a_2_1 = Real(1) / Real(5);
                static const Real a_3_1 = Real(3) / Real(40), a_3_2 = Real(9) / Real(40);
                static const Real a_4_1 = Real(3) / Real(10), a_4_2 = Real(-9) / Real(10),
                                  a_4_3 = Real(6) / Real(5);
                static const Real a_5_1 = Real(-11) / Real(54), a_5_2 = Real(5) / Real(2),
                                  a_5_3 = Real(-70) / Real(27), a_5_4 = Real(35) / Real(27);
                static const Real a_6_1 = Real(1631) / Real(55296), a_6_2 = Real(175) / Real(512),
                                  a_6_3 = Real(575) / Real(13824),
                                  a_6_4 = Real(44275) / Real(110592),
                                  a_6_5 = Real(253) / Real(4096);
                static const Real b4_1 = Real(2825) / Real(27648), b4_2 = Real(0),
                                  b4_3 = Real(18575) / Real(48384),
                                  b4_4 = Real(13525) / Real(55296), b4_5 = Real(277) / Real(14336),
                                  b4_6 = Real(1) / Real(4);
                static const Real b5_1 = Real(37) / Real(378), b5_2 = Real(0),
                                  b5_3 = Real(250) / Real(621), b5_4 = Real(125) / Real(594),
                                  b5_5 = Real(0), b5_6 = Real(512) / Real(1771);

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
