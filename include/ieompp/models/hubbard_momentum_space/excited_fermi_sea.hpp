#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_EXCITED_FERMI_SEA_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_EXCITED_FERMI_SEA_HPP_

#include "ieompp/algebra/operator/get_index.hpp"

#include <algorithm>
#include <list>
#include <utility>
#include <vector>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_momentum_space
        {
            template <typename MonomialT>
            struct ExcitedFermiSea {
                using Monomial = MonomialT;
                using Operator = typename Monomial::Operator;
                using Indices  = typename Operator::Indices;

                bool vanishes = false;
                std::list<Indices> created_particles;
                std::list<Indices> annihilated_particles;

                ExcitedFermiSea() = default;

                template <typename Monomial, typename Dispersion>
                ExcitedFermiSea(const Monomial& monomial, const Dispersion& dispersion,
                                const typename Dispersion::Float& fermi_energy = 0.)
                {
                    for(auto it = monomial.crbegin(); it != monomial.crend(); ++it) {
                        // get indices of next operator
                        const auto indices = algebra::get_indices(*it);

                        // get energy of next creation/annihilation
                        const auto energy = dispersion(std::get<0>(indices));

                        if(it->creator) {
                            // check if creation is below fermi level
                            if(energy < fermi_energy) {
                                // initially the particle exists -> check if annihilation took place
                                auto pos = std::find(annihilated_particles.begin(),
                                                     annihilated_particles.end(), indices);
                                if(pos == annihilated_particles.end()) {
                                    // initial particle still exists -> cannot create another
                                    vanishes = true;
                                    break;
                                } else {
                                    // initial particle was annihilated -> creation is possible
                                    // this reverts the annihilation
                                    annihilated_particles.erase(pos);
                                }
                            } else {
                                // initially the particle does not exist -> check if creation took
                                // place
                                auto pos = std::find(created_particles.begin(),
                                                     created_particles.end(), indices);
                                if(pos != created_particles.end()) {
                                    // particle was already created -> creation is impossible
                                    vanishes = true;
                                    break;
                                } else {
                                    // particle was not yet created
                                    created_particles.push_back(indices);
                                }
                            }
                        } else {
                            // check if annihilation is below fermi level
                            if(energy < fermi_energy) {
                                // initially the particle exists -> check if annihilation took place
                                auto pos = std::find(annihilated_particles.begin(),
                                                     annihilated_particles.end(), indices);
                                if(pos != annihilated_particles.end()) {
                                    // initial particle was already annihilated -> annihilation
                                    // impossible
                                    vanishes = true;
                                    break;
                                } else {
                                    // initial particle was not yet annihilated -> annihilation
                                    // possible
                                    annihilated_particles.push_back(indices);
                                }
                            } else {
                                // initially the particle does not exist -> check if creation took
                                // place
                                auto pos = std::find(created_particles.begin(),
                                                     created_particles.end(), indices);
                                if(pos == created_particles.end()) {
                                    // particle was not created -> annihilation impossible
                                    vanishes = true;
                                    break;
                                } else {
                                    // particle was created -> annihilation possible
                                    created_particles.erase(pos);
                                }
                            }
                        }
                    }

                    if(!vanishes) {
                        created_particles.sort();
                        annihilated_particles.sort();
                    }
                }

                bool operator==(const ExcitedFermiSea& rhs) const
                {
                    if(vanishes || rhs.vanishes) {
                        return false;
                    }
                    if(created_particles.size() != rhs.created_particles.size()) {
                        return false;
                    }
                    if(annihilated_particles.size() != rhs.annihilated_particles.size()) {
                        return false;
                    }
                    return std::equal(created_particles.begin(), created_particles.end(),
                                      rhs.created_particles.begin())
                           && std::equal(annihilated_particles.begin(), annihilated_particles.end(),
                                         rhs.annihilated_particles.begin());
                }

                bool operator!=(const ExcitedFermiSea& rhs) const
                {
                    if(vanishes || rhs.vanishes) {
                        return true;
                    }
                    if(created_particles.size() != rhs.created_particles.size()) {
                        return true;
                    }
                    if(annihilated_particles.size() != rhs.annihilated_particles.size()) {
                        return true;
                    }
                    return !std::equal(created_particles.begin(), created_particles.end(),
                                       rhs.created_particles.begin())
                           || !std::equal(annihilated_particles.begin(),
                                          annihilated_particles.end(),
                                          rhs.annihilated_particles.begin());
                }
            };
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
