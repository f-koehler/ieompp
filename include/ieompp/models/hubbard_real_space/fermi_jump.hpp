#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_FERMI_JUMP_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_FERMI_JUMP_HPP_

#include "ieompp/constants.hpp"
#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/openmp.hpp"
#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/number.hpp"

#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include <iostream>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Float, typename Basis>
            class FermiJump1D
            {
            };

            template <typename FloatT, typename MonomialT>
            class FermiJump1D<FloatT, Basis1Operator<MonomialT>>
            {
            public:
                using Float    = FloatT;
                using Monomial = MonomialT;
                using Operator = typename Monomial::Operator;
                using Basis    = Basis1Operator<Monomial>;
                using Index    = typename Operator::Index1;
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
                    : _N(lattice.size()), _expectation_value(std::move(expectation_value))
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
                    static_cast<void>(basis);

                    std::vector<Complex> results(omp_get_max_threads(), 0.);
#pragma omp parallel for
                    for(Index i = 0; i < _N; ++i) {
                        results[omp_get_thread_num()] += _fourier_coefficients[i] * h[i];
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

            template <typename FloatT, typename MonomialT>
            class FermiJump1D<FloatT, Basis3Operator<MonomialT>>
            {
            public:
                using Float    = FloatT;
                using Monomial = MonomialT;
                using Basis    = Basis3Operator<Monomial>;
                using Operator = typename Monomial::Operator;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;
                using Complex = std::complex<Float>;
                using Index   = typename Operator::Index1;

            private:
                const std::reference_wrapper<const Basis> _basis_ref;
                std::vector<Complex> _fourier_coefficients;
                ExpectationValueFunction _expectation_value;

            public:
                template <typename Lattice>
                FermiJump1D(const Basis& basis, const Lattice& lattice, ExpectationValueFunction ev)
                    : _basis_ref(basis), _fourier_coefficients(basis.N, 0.),
                      _expectation_value(std::move(ev))
                {
                    static const auto k_F = HalfPi<double>::value;

#pragma omp parallel for
                    for(Index i = 0; i < basis.N; ++i) {
                        const auto prod = k_F * lattice[i];

                        // calculate e^{i k_F * r_i}
                        _fourier_coefficients[i] = Complex(std::cos(prod), std::sin(prod));
                    }
                }

                auto& fourier_coefficients() const { return _fourier_coefficients; }
                auto& fourier_coefficients() { return _fourier_coefficients; }

                template <typename Vector>
                Float operator()(const Vector& h) const
                {
                    const auto& basis = _basis_ref.get();
                    static Vector h_NO(basis.N);

#pragma omp parallel for
                    for(Index i = 0; i < basis.N; ++i) {
                        // initialize h_i^NO(t) = h_i(t)
                        h_NO[i] = h[i];
                        for(Index j = 0; j < basis.N; ++j) {
                            for(Index k = 0; k < basis.N; ++k) {
                                // calculate 2*<c_{j,↓}^† c_{k,↓}>
                                auto prefactor =
                                    2 * _expectation_value(basis[j].front(), basis[k].front());

                                // subtract δ_{j,k}
                                if(j == k) {
                                    prefactor -= 1.;
                                }

                                // add (2*<c_{j,↓}^† c_{k,↓}> - δ_{j,k}) * h_{i,j,k}(t) to h_i^NO(t)
                                h_NO[i] += prefactor * h[basis.get_3op_index(i, j, k)];
                            }
                        }
                    }

                    std::vector<Complex> results(omp_get_max_threads(), 0.);
#pragma omp parallel for
                    for(Index i = 0; i < basis.N; ++i) {
                        results[omp_get_thread_num()] += _fourier_coefficients[i] * h_NO[i];
                    }

                    return std::norm(std::accumulate(results.begin(), results.end(), Complex(0.)));
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
