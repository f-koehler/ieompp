#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_MOMENTUM_DISTRIBUTION_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_MOMENTUM_DISTRIBUTION_HPP_

#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/number.hpp"

#include <cmath>
#include <functional>
#include <numeric>
#include <vector>

#include <omp.h>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Basis>
            class MomentumDistribution1D
            {
            };

            template <typename TermT>
            class MomentumDistribution1D<Basis3Operator<TermT>>
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
                MomentumDistribution1D(const Lattice& lattice,
                                       const typename Lattice::Vector& momentum,
                                       ExpectationValueFunction& expectation_value)
                    : _N(lattice.num()), _expectation_value(expectation_value)
                {
                    _fourier_coefficients.reserve(_N);

#pragma omp parallel for
                    for(Index i = 0; i < _N; ++i) {
                        const auto prod          = types::dot_product(momentum, lattice[i]);
                        _fourier_coefficients[i] = Complex(std::cos(prod), std::sin(prod));
                    }
                }

                template <typename Vector>
                const Float operator()(const Basis& basis, const Vector& h) const
                {
                    // buffer to store h^NO coefficients
                    static const auto size = h.size();
                    static Vector h_NO(_N);

// calculate h-coefficients in normally-ordered bases
#pragma omp parallel for
                    for(Index i = 0; i < _N; ++i) {
                        auto& curr = h_NO[i];
                        curr       = h[i];
                        for(Index j = 0; j < _N; ++j) {
                            for(Index k = 0; k < _N; ++k) {
                                const auto idx = basis.get_3op_index(i, j, k);
                                curr += h[idx] * (2 * expectation_value(basis[j], basis[k])
                                                  - ((j == k) ? 1. : 0.));
                            }
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
