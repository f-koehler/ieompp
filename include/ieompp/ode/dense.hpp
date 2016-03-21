#ifndef IEOMPP_ODE_SPARSE_HPP_
#define IEOMPP_ODE_SPARSE_HPP_

#include "Eigen/Dense"

namespace ieompp
{
    namespace ode
    {
        template <typename Agenda>
        class DenseRK4
        {
            public:
                using Term    = typename Agenda::Term;
                using Complex = typename Agenda::Complex;
                using Matrix  = Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>;

            private:
                const std::size_t _dimension;
                Matrix _mat;

            public:
                DenseRK4(const Agenda& agenda)
                    : _dimension(agenda.terms().size()), _mat(Matrix::Zero(_dimension, _dimension))
                {
                    // initialize coefficients
                    std::size_t i = 0;
                    for(auto& row : agenda.results()) {
                        for(auto& e : row) {
                            _mat(e.index, i) = e.prefactor * Complex(0., 1.);
                        }
                        ++i;
                    }
                }

                const Matrix& matrix() const { return _mat; }
        };
    }
}

#endif
