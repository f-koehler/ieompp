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
            template <typename Float, typename Monomial>
            struct ParticleNumber {
            };

            template <typename FloatT, typename MonomialT>
            struct ParticleNumber<FloatT, Basis3Operator<MonomialT>> {
                using Monomial = MonomialT;
                using Basis    = Basis3Operator<Monomial>;
                using Index    = typename Monomial::Operator::Index1;
                using Float    = FloatT;

                const NonVanishingExpectationValues<Index, Float> non_vanishing_expectation_values;

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
                    const auto num             = non_vanishing_expectation_values.size();
                    std::complex<Float> result = 0.;

                    for(std::size_t i = 0; i < num; ++i) {
                        const auto& contribution = non_vanishing_expectation_values[i];

                        if(contribution.left_index == contribution.right_index) {
                            result += std::norm(vec[contribution.left_index]) * contribution.value;
                        } else {
                            result += types::multiply_with_conjugate(vec[contribution.left_index],
                                                                     vec[contribution.right_index])
                                      * contribution.value;
                        }
                    }

                    assert(types::IsZero(result.imag()));

                    return result.real();
                }
            };

            template <typename Basis, typename Dispersion, typename Float>
            ParticleNumber<Float, Basis>
            make_particle_number(const Basis& basis, const Basis& conjugate_basis,
                                 const Dispersion& dispersion, const Float& fermi_energy = 0.)
            {
                return ParticleNumber<Float, Basis>(basis, conjugate_basis, dispersion,
                                                    fermi_energy);
            }
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
