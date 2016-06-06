#ifndef IEOMPP_HUBBARD_HPP_
#define IEOMPP_HUBBARD_HPP_

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include "ieompp/constraints.hpp"
#include "ieompp/dot_product.hpp"
#include "ieompp/exception.hpp"

namespace ieompp
{
    namespace momentum_space
    {
        template <typename PrefactorT>
        struct Hamiltonian {
            using Prefactor = PrefactorT;

            Prefactor J, U;

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_kinetic_terms(const Term& t, const MomentumSpace& space,
                                        const Lattice& lattice, Container& container)
            {
                static_assert(Term::Operator::number_of_indices == 2,
                              "Hubbard model operators have two indices!");
                static_assert(
                    std::is_same<typename MomentumSpace::Vector, typename Lattice::Vector>::value,
                    "Momentum space and lattice should have the same type for their vectors!");
                if(t.operators.size() == 1)
                    generate_kinetic_terms_1(t, space, lattice, container);
                else if(t.operators.size() == 3)
                    generate_kinetic_terms_3(t, space, lattice, container);
                else
                    THROW(NotImplemented, "Currently only implemented for 1- and 3-operator terms");
            }

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_interaction_terms(const Term& t, const MomentumSpace& space,
                                            const Lattice& lattice, Container& container)
            {
                static_assert(Term::Operator::number_of_indices == 2,
                              "Hubbard model operators have two indices!");
                static_assert(
                    std::is_same<typename MomentumSpace::Vector, typename Lattice::Vector>::value,
                    "Momentum space and lattice should have the same type for their vectors!");
                if(t.operators.size() == 1)
                    generate_interaction_terms_1(t, space, lattice, container);
                else if(t.operators.size() == 3)
                    generate_interaction_terms_3(t, space, lattice, container);
                else
                    THROW(NotImplemented, "Currently only implemented for 1- and 3-operator terms");
            }

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
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
                    new_term.prefactpr *= p;
                    container.emplace_back(new_term);
                }
            }

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_kinetic_terms_3(const Term& t, const MomentumSpace& space,
                                          const Lattice& lattice, Container& container)
            {
                const auto op1_creator = t.operators[0].creator;
                const auto op2_creator = t.operators[1].creator;
                const auto op3_creator = t.operators[2].creator;
                if(op1_creator && op2_creator && !op3_creator) {
                } else
                    THROW(NotImplemented, u8"Currenlty only the c^† c^† c structure is supported!");
            }

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_interaction_terms_1(const Term& t, const MomentumSpace& space,
                                              const Lattice& lattice, Container& container)
            {
                using Index  = typename Term::Operator::Index1;
                using Spin   = typename Term::Operator::Index2;
                static_assert(std::is_same<Spin, bool>::value,
                              "Spin has to be bool at the moment!");

                const auto spin = t.operators.front().index2;
                const auto q    = space[t.operators.front().index1];

                if(t.operators.front().creator) {
                    if(!t.operators.front().index2)
                        THROW(NotImplemented, "The case with spin down is not yet implemented!");
                    for(auto k1_idx : space) {
                        const auto k1 = space[k1_idx];
                        for(auto k2_idx : space) {
                            const auto k2 = space[k2_idx];
                            const auto k3 = k1 + k2 - q;
                            const auto k3_idx = space[k3];

                            container.push_back(
                                make_term(t.prefactor * U / lattice.num(),
                                          {make_creator(k1_idx, spin), make_creator(k2_idx, !spin),
                                           make_annihilator(k3_idx, !spin)}));
                        }
                    }
                    // TODO: this term probably vanishes in normalized form
                    Term&& new_term = t;
                    new_term.prefactor *= -U / (2 * lattice.num());
                    container.emplace_back(new_term);
                } else
                    THROW(NotImplemented, "TODO");
            }

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_interaction_terms_3(const Term& t, const MomentumSpace& space,
                                              const Lattice& lattice, Container& container)
            {
                THROW(NotImplemented, "TODO");
            }
        };
    }
}

#endif
