#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_PARTICLE_NUMBER_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_PARTICLE_NUMBER_HPP_

#include "ieompp/models/hubbard_momentum_space/basis.hpp"
#include "ieompp/models/hubbard_momentum_space/non_vanishing_expectation_values.hpp"
#include "ieompp/openmp.hpp"
#include "ieompp/types/number.hpp"

#include <numeric>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_momentum_space
        {
            template <typename Float, typename Basis>
            struct ParticleNumber {
            };

            template <typename FloatT, typename MonomialT>
            struct ParticleNumber<FloatT, Basis3Operator<MonomialT>> {
                using Monomial = MonomialT;
                using Basis    = Basis3Operator<Monomial>;
                using Index    = typename Monomial::Operator::Index1;
                using Float    = FloatT;

                const NonVanishingExpectationValues<Index> non_vanishing_expectation_values;

                template <typename Dispersion>
                ParticleNumber(const Basis3Operator<Monomial>& basis,
                               const Basis3Operator<Monomial>& conjugate_basis,
                               const Dispersion& dispersion, const Float& fermi_energy = 0.)
                    : non_vanishing_expectation_values(basis, conjugate_basis, dispersion,
                                                       fermi_energy)
                {
                }

                template <typename Vector>
                Float operator()(const Vector& vec) const
                {
                    const auto num = non_vanishing_expectation_values.size();
                    std::vector<Float> results(omp_get_max_threads(), 0);

#pragma omp parallel for
                    for(std::size_t i = 0; i < num; ++i) {
                        const auto thread = omp_get_thread_num();
                        const auto& pair  = non_vanishing_expectation_values[i];

                        if(pair.first == pair.second) {
                            results[thread] += std::norm(vec[pair.first]);
                        } else {
                            results[thread] +=
                                types::add_conjugate_products(vec[pair.first], vec[pair.second]);
                        }
                    }

                    return std::accumulate(results.begin(), results.end(), 0.);
                }
            };
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
