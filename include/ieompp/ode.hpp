#ifndef IEOMPP_ODE_HPP_
#define IEOMPP_ODE_HPP_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Dense>
#pragma GCC diagnostic pop

namespace ieompp
{
    template <typename AgendaT>
    class RungeKutta4
    {
        using Agenda = AgendaT;
        using Prefactor = typename Agenda::Prefactor;
        using Matrix = Eigen::Matrix<Prefactor, Eigen::Dynamic, Eigen::Dynamic>;
        using Vector = Eigen::Matrix<Prefactor, Eigen::Dynamic, 1>;

        private:
            Matrix _matrix;
            Vector _current;
            Prefactor _dt, _dt2, _dt6;
            Vector _k1, _k2, _k3, _k4;

        public:
            RungeKutta4(const Agenda& agenda, const Vector& initial, const Prefactor& dt)
            {
                const auto N = agenda.terms().size();
                _matrix      = Matrix::Zero(N, N);
                _current     = initial;
                _dt          = dt;
                _dt2         = _dt / 2.;
                _dt6         = _dt / 6.;
                _k1          = Vector::Zero(N);
                _k2          = Vector::Zero(N);
                _k3          = Vector::Zero(N);
                _k4          = Vector::Zero(N);

                for(std::size_t i = 0; i < N; ++i) {
                    const auto& row = agenda[i];
                    for(auto& coeff : row)
                        _matrix(i, coeff.index) += Prefactor(0, 1) * coeff.prefactor;
                }
            }

            void step() {
                _k1 = _matrix * _current;
                _k2 = _matrix * (_current + _dt2 * _k1);
                _k3 = _matrix * (_current + _dt2 * _k2);
                _k4 = _matrix * (_current + _dt * _k3);
                _current += _dt6 * (_k1 + 2. * _k2 + 2. * _k3 + _k4);
            }

            const Matrix& matrix() const { return _matrix; }
            const Vector& solution() const { return _current; }
    };
}

#endif
