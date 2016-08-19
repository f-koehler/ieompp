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
                            new_term.operators.emplace_back(Operator{true, k1_idx, true});
                            new_term.operators.emplace_back(Operator{true, k2_idx, false});
                            new_term.operators.emplace_back(Operator{false, k3_idx, false});
                            container.emplace_back(new_term);
                        }
                    }
                }

                template <typename Term, typename MomentumSpace, typename Lattice,
                          typename Container>
                void generate_interaction_terms_3(const Term& t, const MomentumSpace& space,
                                                  const Lattice& lattice, Container& container)
                {
                    using Operator = typename Term::Operator;

                    const auto prefactor = t.prefactor * U / lattice.num();

                    const auto q1_idx = t.operators[0].index1;
                    const auto q2_idx = t.operators[1].index1;
                    const auto q3_idx = t.operators[2].index1;
                    const auto q1     = space[q1_idx];
                    const auto q2     = space[q2_idx];
                    const auto q3     = space[q3_idx];

                    // (A) terms
                    for(const auto k1_idx : space) {
                        const auto k1 = space[k1_idx];
                        for(const auto k3_idx : space) {
                            const auto k3 = space[k3_idx];
                            const auto k4 = k1 + k3 - q1;
                            const auto k4_idx = space(k4);

                            if((q2_idx == k3_idx) || (q3_idx == k4_idx)) {
                                if(k3_idx != q3_idx) continue;
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, q2_idx, false});
                                new_term.operators.emplace_back(Operator{false, k4_idx, false});
                                container.emplace_back(new_term);
                                if(q2_idx == k4_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = prefactor / 2;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                                continue;
                            }

                            // q2 != k3 && q3 != k4
                            auto&& new_term = Term();
                            new_term.prefactor = prefactor;
                            new_term.operators.emplace_back(Operator{true, k1_idx, true});
                            new_term.operators.emplace_back(Operator{false, q2_idx, true});
                            new_term.operators.emplace_back(Operator{true, q3_idx, true});
                            new_term.operators.emplace_back(Operator{true, k3_idx, false});
                            new_term.operators.emplace_back(Operator{false, k4_idx, false});
                            container.emplace_back(new_term);

                            if(k3_idx == q3_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = -prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, q2_idx, false});
                                new_term.operators.emplace_back(Operator{false, k4_idx, false});
                                container.emplace_back(new_term);
                            }

                            if(k3_idx == k4_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, q2_idx, false});
                                new_term.operators.emplace_back(Operator{false, q3_idx, false});
                                container.emplace_back(new_term);
                            }

                            if(q2_idx == k4_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, k3_idx, false});
                                new_term.operators.emplace_back(Operator{false, q3_idx, false});
                                container.emplace_back(new_term);
                                if(q3_idx == k3_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = -prefactor / 4;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                            }

                            if(q2_idx == q3_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, k3_idx, false});
                                new_term.operators.emplace_back(Operator{false, k4_idx, false});
                                container.emplace_back(new_term);
                                if(k3_idx == k4_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = prefactor / 4;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                            }
                        }
                    }

                    // (B) terms
                    for(const auto k1_idx : space) {
                        const auto k1 = space[k1_idx];
                        for(const auto k2_idx : space) {
                            const auto k2     = space[k2_idx];
                            const auto k4     = k1 - k2 + q3;
                            const auto k4_idx = space(k4);

                            if(k1_idx == q1_idx) {
                                if(q1_idx != k2_idx) continue;
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, q2_idx, false});
                                new_term.operators.emplace_back(Operator{false, k4_idx, false});
                                container.emplace_back(new_term);
                                if(q2_idx == k4_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = prefactor / 2;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                                continue;
                            }

                            // k1 != q1
                            auto&& new_term = Term();
                            new_term.prefactor = prefactor;
                            new_term.operators.emplace_back(Operator{true, k1_idx, true});
                            new_term.operators.emplace_back(Operator{false, k2_idx, true});
                            new_term.operators.emplace_back(Operator{true, q1_idx, true});
                            new_term.operators.emplace_back(Operator{true, q2_idx, false});
                            new_term.operators.emplace_back(Operator{false, k4_idx, false});
                            container.emplace_back(new_term);

                            if(q2_idx == k4_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{false, k2_idx, true});
                                new_term.operators.emplace_back(Operator{true, q1_idx, true});
                                container.emplace_back(new_term);
                            }

                            if(q1_idx == k2_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = -prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, q2_idx, false});
                                new_term.operators.emplace_back(Operator{false, k4_idx, false});
                                container.emplace_back(new_term);

                                if(q2_idx == k4_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = -prefactor / 4;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                            }

                            if(k1_idx == k2_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, q1_idx, true});
                                new_term.operators.emplace_back(Operator{true, q2_idx, false});
                                new_term.operators.emplace_back(Operator{false, k4_idx, false});
                                container.emplace_back(new_term);

                                if(q2_idx == k4_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = prefactor / 4;
                                    new_term.operators.emplace_back(Operator{true, q1_idx, true});
                                    container.emplace_back(new_term);
                                }
                            }
                        } // k2 loop
                    } // k1 loop

                    // (C) terms
                    for(const auto k1_idx : space) {
                        const auto k1 = space[k1_idx];
                        for(const auto k2_idx : space) {
                            const auto k2     = space[k2_idx];
                            const auto k3     = k2 - k1 + q2;
                            const auto k3_idx = space(k3);

                            if(k1_idx == q1_idx) {
                                if(q1_idx != k2_idx) continue;
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, k3_idx, false});
                                new_term.operators.emplace_back(Operator{false, q3_idx, false});
                                container.emplace_back(new_term);
                                if(q3_idx == k3_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = prefactor / 2;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                                continue;
                            }

                            // k1 != q1
                            auto&& new_term = Term();
                            new_term.prefactor = prefactor;
                            new_term.operators.emplace_back(Operator{true, k1_idx, true});
                            new_term.operators.emplace_back(Operator{false, k2_idx, true});
                            new_term.operators.emplace_back(Operator{true, q1_idx, true});
                            new_term.operators.emplace_back(Operator{true, k3_idx, false});
                            new_term.operators.emplace_back(Operator{false, q3_idx, false});
                            container.emplace_back(new_term);

                            if(q3_idx == k3_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{false, k2_idx, true});
                                new_term.operators.emplace_back(Operator{true, q1_idx, true});
                                container.emplace_back(new_term);
                            }

                            if(q1_idx == k2_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = -prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                new_term.operators.emplace_back(Operator{true, k3_idx, false});
                                new_term.operators.emplace_back(Operator{false, q3_idx, false});
                                container.emplace_back(new_term);

                                if(q3_idx == k3_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = -prefactor / 4;
                                    new_term.operators.emplace_back(Operator{true, k1_idx, true});
                                    container.emplace_back(new_term);
                                }
                            }

                            if(k1_idx == k2_idx) {
                                auto&& new_term = Term();
                                new_term.prefactor = prefactor / 2;
                                new_term.operators.emplace_back(Operator{true, q1_idx, true});
                                new_term.operators.emplace_back(Operator{true, k3_idx, false});
                                new_term.operators.emplace_back(Operator{false, q3_idx, false});
                                container.emplace_back(new_term);

                                if(q3_idx == k3_idx) {
                                    auto&& new_term = Term();
                                    new_term.prefactor = prefactor / 4;
                                    new_term.operators.emplace_back(Operator{true, q1_idx, true});
                                    container.emplace_back(new_term);
                                }
                            }
                        } // k2 loop
                    } // k1 loop

                    // add commutator from left hand side
                    if(q2 == q3) {
                        auto t2 = t;
                        t2.prefactor = t.prefactor / 2;
                        t2.operators.push_back(t.operators.front());
                        generate_interaction_terms_1(t2, space, lattice, container);
                    }
                }
            };

        }
    }
}

#endif
