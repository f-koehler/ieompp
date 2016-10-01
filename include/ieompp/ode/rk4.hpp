#ifndef IEOMPP_ODE_RK4_HPP_
#define IEOMPP_ODE_RK4_HPP_

#include <ieompp/types/eigen/traits.hpp>
#include <ieompp/types/number.hpp>

namespace ieompp
{
    namespace ode
    {
        template <typename RealT>
        class RK4
        {
            using Real = RealT;

        private:
            const std::size_t _N;
            const Real _step_width;

        public:
            RK4(std::size_t dimension, const StepWidth& step_width)
                : _N(dimension), _step_width(step_width)
            {
            }

            const Real& step_width() const { return _dt; }
            std::size_t dimension() const { return _N; }

            template <typename Matrix, typename Vector>
            void step(const Matrix& m, Vector& u)
            {
                assert(types::is_quadratic(m));
                assert(m.rows() == _dimension);
                assert(u.cols() == 1);
                assert(u.rows() == _dimension);

                _k1 = m * u;
                _k2 = m * (u + (_dt / 2) * _k1);
                _k3 = m * (u + (_dt / 2) * _k2);
                _k4 = m * (u + _dt * _k3);
                u += (_step_width / 6.) * (_k1 + 2. * _k2 + 2. * _k3 + _k4);
            }
        };

        namespace io
        {
            template <typename FileInfoHeader, typename Matrix, typename Vector>
            FileInfoHeader& add_file_info(FileInfoHeader& fih, const RK4<Matrix, Vector>& rk4)
            {
                fih.add_entry("Integrator");
                fih.add_entry("  type", "RungeKutta4 (fixed step width)");
                fih.add_entry("  dimension", rk4.dimension());
                fih.add_entry("  step width", rk4.step_width());
                return fih;
            }
        }
    }
}

#endif
