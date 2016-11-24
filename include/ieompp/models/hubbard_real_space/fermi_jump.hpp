#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_FERMI_JUMP_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_FERMI_JUMP_HPP_

#include "ieompp/constants.hpp"
#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/openmp.hpp"
#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/number.hpp"

#include <cmath>
#include <functional>
#include <numeric>
#include <utility>
#include <vector>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Basis>
            class FermiJump1D
            {
            };

            template <typename TermT>
            class FermiJump1D<Basis3Operator<TermT>>
            {
            public:
                using Term     = TermT;
                using Operator = typename Term::Operator;
                using Basis    = Basis3Operator<Term>;
                using Index    = typename Operator::Index1;
                using Float    = typename types::RealType<typename Term::Prefactor>::Type;
                using Complex  = std::complex<Float>;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;

            private:
                std::vector<Complex> _fourier_coefficients;
                const Index _N;
                const ExpectationValueFunction _expectation_value;

            public:
                template <typename Lattice>
                FermiJump1D(const Lattice& lattice, ExpectationValueFunction expectation_value)
                    : _N(lattice.num()), _expectation_value(std::move(expectation_value))
                {
                    static const auto k_F = HalfPi<double>::value;

                    _fourier_coefficients.reserve(_N);

#pragma omp parallel for
                    for(Index i = 0; i < _N; ++i) {
                        const auto prod          = k_F * lattice[i];
                        _fourier_coefficients[i] = Complex(std::cos(prod), std::sin(prod));
                    }
                }

                template <typename Vector>
                const Float operator()(const Basis& basis, const Vector& h) const
                {
                    // buffer to store h^NO coefficients
                    static Vector h_NO(_N);

#pragma omp parallel for
                    // calculate h-coefficients in normally-ordered bases
                    for(Index i = 0; i < _N; ++i) {
                        auto& curr = h_NO[i];
                        curr       = h[i];
                        for(Index j = 0; j < _N; ++j) {
                            const auto& op_j_0 = basis[j].operators.front();
                            for(Index k = 0; k < j; ++k) {
                                const auto idx = basis.get_3op_index(i, j, k);
                                curr += h[idx] * 4.
                                        * _expectation_value(op_j_0, basis[k].operators.front());
                            }
                            const auto idx = basis.get_3op_index(i, j, j);
                            curr += h[idx] * (2. * _expectation_value(op_j_0, op_j_0) - 1.);
                        }
                    }

                    std::vector<Complex> results(omp_get_max_threads(), 0.);

#pragma omp parallel for
                    for(Index i = 0; i < _N; ++i) {
                        results[omp_get_thread_num()] += _fourier_coefficients[i] * h_NO[i];
                    }

                    return std::norm(
                        std::accumulate(results.begin(), results.end(), Complex(0., 0.)));
                }

                const Complex& fourier_coefficient(const Index& index) const
                {
                    return _fourier_coefficients[index];
                }

                const Float expectation_value(const Operator& a, const Operator& b)
                {
                    return _expectation_value(a, b);
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
