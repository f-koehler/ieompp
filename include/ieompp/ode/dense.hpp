#ifndef IEOMPP_ODE_SPARSE_HPP_
#define IEOMPP_ODE_SPARSE_HPP_

#include "Eigen/Dense"

namespace ieompp
{
    namespace ode
    {
        template <typename System>
        class DenseRK4
        {
            public:
                using Term    = typename System::Term;
                using Complex = typename System::Complex;
                using Real    = typename Complex::value_type;
                using Matrix  = Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>;
                using Vector  = Eigen::Matrix<Complex, Eigen::Dynamic, 1>;

            private:
                const std::size_t _dimension;
                Matrix _mat;

            public:
                DenseRK4(const System& system)
                    : _dimension(system.terms.size()), _mat(Matrix::Zero(_dimension, _dimension))
                {
                    // initialize coefficients
                    std::size_t i = 0;
                    for(auto& row : system.coefficients) {
                        for(auto& e : row) {
                            _mat(i, e.index) = e.prefactor * Complex(0., 1.);
                        }
                        ++i;
                    }
                }

                void step(Vector& v, const Real& h)
                {
                    const auto h2 = h / 2.;
                    auto _k1      = _mat * v;
                    auto _k2      = v + h2 * _mat * _k1;
                    auto _k3      = v + h2 * _mat * _k2;
                    auto _k4 = v + h * _mat * _k3;
                    v += (h / 6.) * (_k1 + 2 * _k2 + 2 * _k3 + _k4);
                }

                const Matrix& matrix() const { return _mat; }
        };
    }
}

#endif
