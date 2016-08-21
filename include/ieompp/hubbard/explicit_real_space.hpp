#ifndef IEOMPP_HUBBARD_EXPLICIT_HPP_
#define IEOMPP_HUBBARD_EXPLICIT_HPP_

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include <ieompp/constraints.hpp>
#include <ieompp/exception.hpp>
#include <ieompp/types/dot_product.hpp>
#include <ieompp/hubbard/dispersion.hpp>

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
                    auto num_operators = t.operators.size();
                    if(num_operators == 1) {
                        assert(t.operators.front().creator);
                        assert(t.operators.front().index2);
                        generate_kinetic_terms_1(t, lattice, container);
                    } else if(num_operators == 3) {
                        assert(t.operators[0].creator && t.operators[1].creator
                               && !t.operators[2].creator);
                        assert(t.operators[0].index2 && !t.operators[1].index2
                               && !t.operators[2].index2);
                        generate_kinetic_terms_3(t, lattice, container);
                    } else
                        THROW(NotImplemented,
                              "Currently only implemented for 1- and 3-operator terms");
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_interaction_terms(const Term& t, const Lattice& lattice,
                                                Container& container)
                {
                    auto num_operators = t.operators.size();
                    if(num_operators == 1) {
                        assert(t.operators.front().creator);
                        assert(t.operators.front().index2);
                        generate_interaction_terms_1(t, lattice, container);
                    } else if(num_operators == 3) {
                        assert(t.operators[0].creator && t.operators[1].creator
                               && !t.operators[2].creator);
                        assert(t.operators[0].index2 && !t.operators[1].index2
                               && !t.operators[2].index2);
                        generate_interaction_terms_3(t, lattice, container);
                    } else
                        THROW(NotImplemented,
                              "Currently only implemented for 1- and 3-operator terms");
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_kinetic_terms_1(const Term& t, const Lattice& lattice,
                                              Container& container)
                {
                    using Operator = typename Term::Operator;

                    const auto r_idx     = t.operators.front().index1;
                    const auto r         = lattice[r_idx];
                    const auto prefactor = -J * t.prefactor;
                    for(const auto& delta : lattice.lattice_vectors()) {
                        container.emplace_back(
                            Term{prefactor, {Operator{true, lattice(r + delta), true}}});
                        container.emplace_back(
                            Term{prefactor, {Operator{true, lattice(r - delta), true}}});
                    }
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_kinetic_terms_3(const Term& t, const Lattice& lattice,
                                              Container& container)
                {
                    using Operator = typename Term::Operator;

                    const auto r_idx     = t.operators.front().index1;
                    const auto r         = lattice[r_idx];
                    const auto prefactor = -J * t.prefactor;
                    for(const auto& delta : lattice.lattice_vectors()) {
                        container.emplace_back(
                            Term{prefactor,
                                 {Operator{true, lattice(r - delta), true},
                                  Operator{true, r_idx, false}, Operator{false, r_idx, false}}});
                        container.emplace_back(
                            Term{prefactor,
                                 {Operator{true, lattice(r + delta), true},
                                  Operator{true, r_idx, false}, Operator{false, r_idx, false}}});
                        container.emplace_back(Term{prefactor,
                                                    {Operator{true, r_idx, true},
                                                     Operator{true, lattice(r - delta), false},
                                                     Operator{false, r_idx, false}}});
                        container.emplace_back(Term{prefactor,
                                                    {Operator{true, r_idx, true},
                                                     Operator{true, lattice(r + delta), false},
                                                     Operator{false, r_idx, false}}});
                        container.emplace_back(
                            Term{-prefactor,
                                 {Operator{true, r_idx, true}, Operator{true, r_idx, false},
                                  Operator{false, lattice(r - delta), false}}});
                        container.emplace_back(
                            Term{-prefactor,
                                 {Operator{true, r_idx, true}, Operator{true, r_idx, false},
                                  Operator{false, lattice(r + delta), false}}});
                    }
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_interaction_terms_1(const Term& t, const Lattice& lattice,
                                                  Container& container)
                {
                    (void)lattice;
                    using Operator = typename Term::Operator;

                    const auto r_idx     = t.operators.front().index1;
                    const auto prefactor = U * t.prefactor;

                    container.emplace_back(
                        Term{prefactor,
                             {Operator{true, r_idx, true}, Operator{true, r_idx, false},
                              Operator{false, r_idx, false}}});
                    container.emplace_back(Term{prefactor / 2., {Operator{true, r_idx, true}}});
                }

                template <typename Term, typename Lattice, typename Container>
                void generate_interaction_terms_3(const Term& t, const Lattice& lattice,
                                                  Container& container)
                {
                    (void)lattice;
                    using Operator = typename Term::Operator;

                    const auto r_idx     = t.operators.front().index1;
                    const auto prefactor = U * t.prefactor;

                    container.emplace_back(
                        Term{prefactor,
                             {Operator{true, r_idx, true}, Operator{true, r_idx, false},
                              Operator{false, r_idx, false}}});
                    container.emplace_back(Term{prefactor, {Operator{true, r_idx, true}}});
                }
            };
        }
    }
}

#endif
