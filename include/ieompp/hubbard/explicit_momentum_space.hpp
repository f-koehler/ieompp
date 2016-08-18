#ifndef IEOMPP_HUBBARD_EXPLICIT_MOMENTUM_SPACE_HPP_
#define IEOMPP_HUBBARD_EXPLICIT_MOMENTUM_SPACE_HPP_

#include <cassert>
#include <cmath>
#include <set>
#include <type_traits>
#include <vector>

#include <ieompp/algebra/term_comparison.hpp>
#include <ieompp/constraints.hpp>
#include <ieompp/exception.hpp>
#include <ieompp/hubbard/dispersion.hpp>
#include <ieompp/types/dot_product.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace momentum_space
        {
            template <typename PrefactorT>
            struct Hamiltonian {
                using Prefactor = PrefactorT;

                Prefactor J, U;

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_terms(const Term& t, const MomentumSpace& space,
                                    const Lattice& lattice, Container& container)
                {
                    generate_kinetic_terms(t, space, lattice, container);
                    generate_interaction_terms(t, space, lattice, container);
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_kinetic_terms(const Term& t, const MomentumSpace& space,
                                            const Lattice& lattice, Container& container)
                {
                    auto num_operators = t.operators.size();
                    if(num_operators == 1) {
                        assert(t.operators.front().creator);
                        generate_kinetic_terms_1(t, space, lattice, container);
                    } else if(num_operators == 3) {
                        assert(t.operators[0].creator && t.operators[1].creator && !t.operators[2].creator);
                        generate_kinetic_terms_3(t, space, lattice, container);
                    } else THROW(NotImplemented, "");
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms(const Term& t, const MomentumSpace& space,
                                                const Lattice& lattice, Container& container)
                {
                    auto num_operators = t.operators.size();
                    if(num_operators == 1) {
                        assert(t.operators.front().creator);
                        assert(t.operators.front().index2);
                        generate_interaction_terms_1(t, space, lattice, container);
                    } else if(num_operators == 3) {
                        assert(t.operators[0].creator && t.operators[1].creator && !t.operators[2].creator);
                        assert(t.operators[0].index2 && !t.operators[1].index2 && !t.operators[2].index2);
                        generate_interaction_terms_3(t, space, lattice, container);
                    } else THROW(NotImplemented, "");
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_kinetic_terms_1(const Term& t, const MomentumSpace& space,
                                              const Lattice& lattice, Container& container)
                {
                    Dispersion<typename MomentumSpace::Vector, Lattice> dispersion{J};
                    auto&& new_term = Term(t);
                    new_term.prefactor *= -dispersion(space[t.operators.front().index1], lattice);
                    container.emplace_back(new_term);
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_kinetic_terms_3(const Term& t, const MomentumSpace& space,
                                              const Lattice& lattice, Container& container)
                {
                    Dispersion<typename MomentumSpace::Vector, Lattice> dispersion{J};
                    auto&& new_term = Term(t);
                    new_term.prefactor *= (dispersion(space[t.operators[1].index1], lattice)
                                    - dispersion(space[t.operators[0].index1], lattice)
                                    - dispersion(space[t.operators[2].index1], lattice));
                    container.emplace_back(new_term);
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms_1(const Term& t, const MomentumSpace& space,
                                                  const Lattice& lattice, Container& container)
                {
                    using Operator = typename Term::Operator;
                    auto q_idx = t.operators.front().index1;
                    auto q     = space[q_idx];
                    for(auto k1_idx : space) {
                        auto k1 = space[k1_idx];
                        for(auto k2_idx : space) {
                            auto k2         = space[k2_idx];
                            auto k3         = k1 + k2 - q;
                            auto k3_idx     = space(k3);
                            auto&& new_term = Term();
                            new_term.prefactor = (U / lattice.num()) * t.prefactor;
                            new_term.operators.push_back(Operator{true, k1_idx, true});
                            new_term.operators.push_back(Operator{true, k2_idx, false});
                            new_term.operators.push_back(Operator{false, k3_idx, false});
                            container.emplace_back(new_term);
                        }
                    }
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms_3(const Term& t, const MomentumSpace& space,
                                                  const Lattice& lattice, Container& container)
                {
                }
            };
        }
    }
}

#endif
