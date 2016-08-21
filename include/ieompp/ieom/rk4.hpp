#ifndef IEOMPP_IEOM_RK4_HPP_
#define IEOMPP_IEOM_RK4_HPP_

#include <ieompp/ieom/dynamical_system.hpp>

namespace ieompp
{
    namespace ieom
    {
        template <typename TermT>
        struct RK4
        {
            public:
                using Term      = TermT;
                using Prefactor = typename Term::Prefactor;
                using Matrix    = Eigen::Matrix<Prefactor, Eigen::Dynamic, Eigen::Dynamic>;
                using Vector    = Eigen::Matrix<Prefactor, Eigen::Dynamic, 1>;

                double dt;

                void step(const Matrix& m, Vector& v) {
                    static auto dt2 = dt / 2.;
                    static Vector k1, k2, k3, k4;
                    dt2 = dt / 2.;
                    k1  = m * v;
                    k2  = m * (v + dt2 * k1);
                    k3  = m * (v + dt2 * k2);
                    k4  = m * (v + dt * k3);
                    v += (dt / 6.) * (k1 + 2 * k2 + 2 * k3 + k4);
                }
        };
    }
}

#endif
