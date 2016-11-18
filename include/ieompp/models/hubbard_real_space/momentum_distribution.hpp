#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_MOMENTUM_DISTRIBUTION_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_MOMENTUM_DISTRIBUTION_HPP_

#include "ieompp/types/dot_product.hpp"
#include "ieompp/types/number.hpp"

#include <cmath>
#include <numeric>
#include <vector>

#include <omp.h>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename FloatT, typename IndexT>
            class MomentumDistribution1D
            {
            public:
                using Index   = IndexT;
                using Float   = FloatT;
                using Complex = std::complex<Float>;

            private:
                std::vector<Complex> _fourier_coefficients;
                const Index _N;

            public:
                template <typename Lattice>
                MomentumDistribution1D(const Lattice& lattice,
                                       const typename Lattice::Vector& momentum)
                    : _N(lattice.num())
                {
                    _fourier_coefficients.reserve(_N);

#pragma omp parallel for
                    for(Index i = 0; i < _N; ++i) {
                        const auto prod          = types::dot_product(momentum, lattice[i]);
                        _fourier_coefficients[i] = Complex(std::cos(prod), std::sin(prod));
                    }
                }

                template <typename Vector>
                const Float operator()(const Vector& h) const
                {
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
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
