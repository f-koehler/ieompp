#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_EXCITED_FERMI_SEA_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_EXCITED_FERMI_SEA_HPP_

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
            template <typename IndexT>
            struct ExcitedFermiSea {
                using Index = IndexT;

                bool vanishes = false;
                std::list<Index> created_particles;
                std::list<Index> annihilated_particles;

                ExcitedFermiSea() = default;

                template <typename Term, typename Dispersion>
                ExcitedFermiSea(const Term& term, const Dispersion& dispersion,
                                const typename Dispersion::Float& fermi_energy = 0.)
                {
                    const auto& ops = term.operators;

                    for(auto it = ops.crbegin(); it != ops.crend(); ++it) {
                        // get energy of next creation/annihilation
                        const auto energy = dispersion[it->index1];

                        if(it->creator) {
                            // check if creation is below fermi level
                            if(energy < fermi_energy) {
                                // initially the particle exists -> check if annihilation took place
                                auto pos = std::find(annihilated_particles.begin(),
                                                     annihilated_particles.end(), it->index1);
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
                                                     created_particles.end(), it->index1);
                                if(pos != created_particles.end()) {
                                    // particle was already created -> creation is impossible
                                    vanishes = true;
                                    break;
                                } else {
                                    // particle was not yet created
                                    created_particles.push_back(it->index1);
                                }
                            }
                        } else {
                            // check if annihilation is below fermi level
                            if(energy < fermi_energy) {
                                // initially the particle exists -> check if annihilation took place
                                auto pos = std::find(annihilated_particles.begin(),
                                                     annihilated_particles.end(), it->index1);
                                if(pos != annihilated_particles.end()) {
                                    // initial particle was already annihilated -> annihilation
                                    // impossible
                                    vanishes = true;
                                    break;
                                } else {
                                    // initial particle was not yet annihilated -> annihilation
                                    // possible
                                    annihilated_particles.push_back(it->index1);
                                }
                            } else {
                                // initially the particle does not exist -> check if creation took
                                // place
                                auto pos = std::find(created_particles.begin(),
                                                     created_particles.end(), it->index1);
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
            };

            template <typename Index>
            bool have_vanishing_overlap(const ExcitedFermiSea<Index>& left_state,
                                        const ExcitedFermiSea<Index>& right_state)
            {
                // the states are orthogonal if all creations/annihilations cancel out
                if(left_state.created_particles.size()
                   != right_state.annihilated_particles.size()) {
                    return true;
                }
                if(left_state.annihilated_particles.size()
                   != right_state.created_particles.size()) {
                    return true;
                }
                return !std::equal(left_state.created_particles.begin(),
                                   left_state.created_particles.end(),
                                   right_state.annihilated_particles.begin())
                       || !std::equal(left_state.annihilated_particles.begin(),
                                      left_state.annihilated_particles.end(),
                                      right_state.created_particles.begin());
            }
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
