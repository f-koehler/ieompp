#ifndef IEOMPP_HUBBARD_EXPLICIT_HPP_
#define IEOMPP_HUBBARD_EXPLICIT_HPP_

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include "ieompp/constraints.hpp"
#include "ieompp/dot_product.hpp"
#include "ieompp/exception.hpp"

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
                void generate_kinetic_terms(const Term& t, const MomentumSpace& space,
                                            const Lattice& lattice, Container& container)
                {
                    static_assert(Term::Operator::number_of_indices == 2,
                                  "Hubbard model operators have two indices!");
                    static_assert(
                        std::is_same<typename MomentumSpace::Vector,
                                     typename Lattice::Vector>::value,
                        "Momentum space and lattice should have the same type for their vectors!");
                    if(t.operators.size() == 1)
                        generate_kinetic_terms_1(t, space, lattice, container);
                    else if(t.operators.size() == 3)
                        generate_kinetic_terms_3(t, space, lattice, container);
                    else
                        THROW(NotImplemented,
                              "Currently only implemented for 1- and 3-operator terms");
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms(const Term& t, const MomentumSpace& space,
                                                const Lattice& lattice, Container& container)
                {
                    static_assert(Term::Operator::number_of_indices == 2,
                                  "Hubbard model operators have two indices!");
                    static_assert(
                        std::is_same<typename MomentumSpace::Vector,
                                     typename Lattice::Vector>::value,
                        "Momentum space and lattice should have the same type for their vectors!");
                    if(t.operators.size() == 1)
                        generate_interaction_terms_1(t, space, lattice, container);
                    else if(t.operators.size() == 3)
                        generate_interaction_terms_3(t, space, lattice, container);
                    else
                        THROW(NotImplemented,
                              "Currently only implemented for 1- and 3-operator terms");
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_kinetic_terms_1(const Term& t, const MomentumSpace& space,
                                              const Lattice& lattice, Container& container)
                {
                    if(t.operators.front().creator) {
                        auto momentum = space[t.operators.front().index1];

                        // TODO: check for type conversion
                        Prefactor p = 0.;
                        for(auto& lattice_vector : lattice.lattice_vectors())
                            p += std::cos(dot_product(momentum, lattice_vector));
                        p *= -2 * J;

                        Term&& new_term = t;
                        new_term.prefactpr *= p;
                        container.emplace_back(new_term);
                    } else {
                        auto momentum = space[t.operators.front().index1];

                        // TODO: check for type conversion
                        Prefactor p = 0.;
                        for(auto& lattice_vector : lattice.lattice_vectors())
                            p += std::cos(dot_product(momentum, lattice_vector));
                        p *= +2 * J;

                        Term&& new_term = t;
                        new_term.prefactor *= p;
                        container.emplace_back(new_term);
                    }
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_kinetic_terms_3(const Term& t, const MomentumSpace& space,
                                              const Lattice& lattice, Container& container)
                {
                    const auto op1_creator = t.operators[0].creator;
                    const auto op2_creator = t.operators[1].creator;
                    const auto op3_creator = t.operators[2].creator;
                    if(op1_creator && op2_creator && !op3_creator) {
                        const auto k1 = space[t.operators[0].index1];
                        const auto k2 = space[t.operators[1].index1];
                        const auto k3 = space[t.operators[2].index1];

                        Prefactor p;
                        for(auto& lattice_vector : lattice.lattice_vectors())
                            p += std::cos(dot_product(k1, lattice_vector))
                                 + std::cos(dot_product(k2, lattice_vector))
                                 - std::cos(dot_product(k3, lattice_vector));
                        p *= -2 * J;

                        // TODO: calc prefactor direclty in new_term prefactor
                        Term&& new_term    = t;
                        new_term.prefactor = p;
                        container.emplace_back(new_term);
                    } else
                        THROW(NotImplemented,
                              u8"Currenlty only the c^† c^† c structure is supported!");
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms_1(const Term& t, const MomentumSpace& space,
                                                  const Lattice& lattice, Container& container)
                {
                    using Index = typename Term::Operator::Index1;
                    using Spin  = typename Term::Operator::Index2;
                    static_assert(std::is_same<Spin, bool>::value,
                                  "Spin has to be bool at the moment!");

                    const auto spin = t.operators.front().index2;
                    const auto q    = space[t.operators.front().index1];

                    if(t.operators.front().creator) {
                        if(!t.operators.front().index2)
                            THROW(NotImplemented,
                                  "The case with spin down is not yet implemented!");
                        for(auto k1_idx : space) {
                            const auto k1 = space[k1_idx];
                            for(auto k2_idx : space) {
                                const auto k2     = space[k2_idx];
                                const auto k3     = k1 + k2 - q;
                                const auto k3_idx = space[k3];

                                container.push_back(make_term(t.prefactor * U / (2 * lattice.num()),
                                                              {make_creator(k1_idx, spin),
                                                               make_creator(k2_idx, !spin),
                                                               make_annihilator(k3_idx, !spin)}));
                            }
                        }
                    } else
                        THROW(NotImplemented, "TODO");
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms_3(const Term& t, const MomentumSpace& space,
                                                  const Lattice& lattice, Container& container)
                {
                    using Index = typename Term::Operator::Index1;
                    using Spin  = typename Term::Operator::Index2;
                    static_assert(std::is_same<Spin, bool>::value,
                                  "Spin has to be bool at the moment!");

                    const auto& op1        = t.operators[0];
                    const auto& op2        = t.operators[1];
                    const auto& op3        = t.operators[2];
                    const auto op1_creator = op1.creator;
                    const auto op2_creator = op2.creator;
                    const auto op3_creator = op3.creator;
                    const auto N           = lattice.num();
                    if(!(op1_creator && op2_creator && !op3_creator))
                        THROW(NotImplemented,
                              u8"Currenlty only the c^† c^† c structure is supported!");
                    if(!(op1.index2 && !op2.index2 && !op3.index2))
                        THROW(NotImplemented,
                              u8"only ↑↓↓ configurations is currently implemented!");

                    for(auto k1_idx : space) {
                        const auto k3_idx = op3.index1;
                        const auto k2     = space[op2.index1] + space[op1.index1] - space[k1_idx];
                        const auto k2_idx = space[k2];
                        auto prefactor    = t.prefactor * U;
                        if(k1_idx != op1.index1)
                            prefactor /= 2 * N;
                        else
                            prefactor /= N;
                        container.push_back(make_term(
                            prefactor, {make_creator(k1_idx, true), make_creator(k2_idx, false),
                                        make_annihilator(k3_idx, false)}));
                    }
                }
            };
        }
    }
}

#endif
