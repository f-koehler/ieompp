#ifndef IEOMPP_HUBBARD_EXPLICIT_HPP_
#define IEOMPP_HUBBARD_EXPLICIT_HPP_

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include <ieompp/constraints.hpp>
#include <ieompp/exception.hpp>
#include <ieompp/types/dot_product.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename PrefactorT>
            struct Hamiltonian {
                using Prefactor = PrefactorT;

                Prefactor J, U;

                template <typename Term, typename Lattice, typename Container>
                void generate_terms(const Term& t, const Lattice& lattice, Container& container)
                {
                    generate_kinetic_terms(t, lattice, container);
                    generate_interaction_terms(t, lattice, container);
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_kinetic_terms(const Term& t, const Lattice& lattice,
                                            Container& container)
                {
                    static_assert(Term::Operator::number_of_indices == 2,
                                  "Hubbard model operators have two indices!");
                    if(t.operators.size() == 1)
                        generate_kinetic_terms_1(t, lattice, container);
                    else if(t.operators.size() == 3)
                        generate_kinetic_terms_3(t, lattice, container);
                    else
                        THROW(NotImplemented,
                              "Currently only implemented for 1- and 3-operator terms");
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_interaction_terms(const Term& t, const Lattice& lattice,
                                                Container& container)
                {
                    static_assert(Term::Operator::number_of_indices == 2,
                                  "Hubbard model operators have two indices!");
                    if(t.operators.size() == 1)
                        generate_interaction_terms_1(t, lattice, container);
                    else if(t.operators.size() == 3)
                        generate_interaction_terms_3(t, lattice, container);
                    else
                        THROW(NotImplemented,
                              "Currently only implemented for 1- and 3-operator terms");
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_kinetic_terms_1(const Term& t, const Lattice& lattice,
                                              Container& container)
                {
                    const auto r = lattice[t.operators.front().index1];
                    if(t.operators.front().creator) {
                        auto new_term = t;
                        new_term.prefactor *= -J;
                        for(auto lattice_vector : lattice.lattice_vectors()) {
                            new_term.operators.front().index1 = lattice(r - lattice_vector);
                            container.push_back(new_term);
                            new_term.operators.front().index1 = lattice(r + lattice_vector);
                            container.push_back(new_term);
                        }
                    } else {
                        THROW(NotImplemented, "TODO");
                    }
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_kinetic_terms_3(const Term& t, const Lattice& lattice,
                                              Container& container)
                {
                    (void)t;
                    (void)lattice;
                    (void)container;
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_interaction_terms_1(const Term& t, const Lattice& lattice,
                                                  Container& container)
                {
                    (void)lattice;
                    if(t.operators.front().creator) {
                        auto new_term = t;
                        new_term.prefactor *= U;
                        new_term.operators.push_back(new_term.operators.front());
                        new_term.operators.back().index2 = !new_term.operators.back().index2;

                        new_term.operators.push_back(new_term.operators.back());
                        new_term.operators.back().creator = false;

                        container.push_back(new_term);
                    } else {
                        THROW(NotImplemented, "TODO");
                    }
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_interaction_terms_3(const Term& t, const Lattice& lattice,
                                                  Container& container)
                {
                    (void)lattice;

                    const auto op1_creator = t.operators[0].creator;
                    const auto op2_creator = t.operators[1].creator;
                    const auto op3_creator = t.operators[2].creator;

                    if(op1_creator && op2_creator && !op3_creator) {
                        auto new_term = t;
                        new_term.prefactor *= U / 2;
                        container.push_back(new_term);
                        new_term.prefactor = t.prefactor * U;
                        new_term.operators.erase(new_term.operators.begin() + 1,
                                                 new_term.operators.end());
                        container.push_back(new_term);
                    } else
                        THROW(NotImplemented,
                              u8"Currently only the c^† c^† c structure is supported!");
                }
            };
        }
    }
}

#endif
