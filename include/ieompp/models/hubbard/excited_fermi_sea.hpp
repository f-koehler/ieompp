#ifndef IEOMPP_MODELS_HUBBARD_EXCITED_FERMI_SEA_HPP_
#define IEOMPP_MODELS_HUBBARD_EXCITED_FERMI_SEA_HPP_

#include <algorithm>
#include <list>
#include <utility>
#include <vector>

namespace ieompp
{
    namespace hubbard
    {
        namespace momentum_space
        {
            template <typename IndexT>
            struct ExcitedFermiSea {
                using Index = IndexT;

                bool vanishes = false;
                std::list<Index> created_particles;
                std::list<Index> annihilated_particles;

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
                                // particle already exists if no annihilation took place
                                auto find = find_annihilation(it->index1);
                                if(!find.first) {
                                    // cannot create another particle
                                    vanishes = true;
                                    break;
                                } else {
                                    // creation reverts previous annihilation
                                    annihilated_particles.remove(find.second);
                                }
                            } else {
                                // particle does not exist unless creation took place
                                auto find = find_creation(it->index1);
                                if(find.first) {
                                    // cannot create another particle
                                    vanishes = true;
                                    break;
                                } else {
                                    // create new particle
                                    created_particles.push_back(it->index1);
                                }
                            }
                        } else {
                            // check if annihilation is below fermi level
                            if(energy < fermi_energy) {
                                // particle does exist unless annihilation took place
                                auto find = find_annihilation(it->index1);
                                if(find.first) {
                                    // annihilation of annihilated particle fails
                                    vanishes = true;
                                    break;
                                } else {
                                    // annihilate present particle
                                    annihilated_particles.push_back(it->index2);
                                }
                            } else {
                                // particle does not exist if no creation took place
                                auto find = find_creation(it->index1);
                                if(!find.first) {
                                    // annihilation of absent particle fails
                                    vanishes = true;
                                    break;
                                } else {
                                    // annihilate created particle
                                    created_particles.remove(find.second);
                                }
                            }
                        }
                    }

                    if(!vanishes) {
                        std::sort(created_particles.begin(), created_particles.end());
                        std::sort(annihilated_particles.begin(), annihilated_particles.end());
                    }
                }

                auto find_creation(const Index& site)
                {
                    auto it = std::find(created_particles.begin(), created_particles.end(), site);
                    return std::make_pair(it == created_particles.end(), it);
                }

                auto find_annihilation(const Index& site)
                {
                    auto it = std::find(created_particles.begin(), created_particles.end(), site);
                    return std::make_pair(it == created_particles.end(), it);
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
                                      right_state.created_particles());
            }
        }
    }
}

#endif
