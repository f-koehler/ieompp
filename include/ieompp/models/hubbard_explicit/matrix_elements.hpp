#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_ELEMENTS_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_ELEMENTS_HPP_

#include <cassert>
#include <ieompp/description.hpp>
#include <ieompp/models/hubbard_explicit/dispersion.hpp>
#include <ieompp/models/hubbard_explicit/operator.hpp>

namespace ieompp
{
    namespace hubbard
    {
        template <typename Term>
        bool valid_1op_term(const Term& t)
        {
            return (t.operators.size() == 1) && t.operators[0].creator && t.operators[0].index2;
        }

        template <typename Term>
        bool valid_3op_term(const Term& t)
        {
            return (t.operators.size() == 3) && t.operators[0].creator && t.operators[1].creator
                   && !t.operators[2].creator && t.operators[0].index2 && !t.operators[1].index2
                   && !t.operators[2].index2;
        }

        namespace real_space
        {
            template <typename PrefactorT>
            struct MatrixElements {
                using Prefactor = PrefactorT;

                const Prefactor J = 1., U = 1.;

                template <typename Term, typename Lattice>
                Prefactor hopping(const Term& row_term, const Term& col_term,
                                  const Lattice& lattice) const;

                template <typename Term>
                Prefactor interaction(const Term& row_term, const Term& col_term) const;
            };

            template <typename Prefactor>
            template <typename Term, typename Lattice>
            Prefactor MatrixElements<Prefactor>::hopping(const Term& row_term, const Term& col_term,
                                                         const Lattice& lattice) const
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");
                const auto len_row = row_term.operators.size();
                const auto len_col = col_term.operators.size();

                assert(valid_1op_term(row_term) || valid_3op_term(row_term));
                assert(valid_1op_term(col_term) || valid_3op_term(col_term));

                const auto& row_ops = row_term.operators;
                const auto& col_ops = col_term.operators;

                if(len_row == 1) {
                    if(len_col == 1) {
                        if(lattice.neighboring(row_ops[0].index1, col_ops[0].index1)) return -J;
                        return 0.;
                    }
                    return 0.;
                } else {
                    if(len_col == 1) {
                        return 0.;
                    }
                    if((row_ops[1].index1 == col_ops[1].index1)
                       && (row_ops[2].index1 == col_ops[2].index1)
                       && lattice.neighboring(row_ops[0].index1, col_ops[0].index1)) {
                        return -J;
                    }
                    if((row_ops[0].index1 == col_ops[0].index1)
                       && (row_ops[2].index1 == col_ops[2].index1)
                       && lattice.neighboring(row_ops[1].index1, col_ops[1].index1)) {
                        return -J;
                    }
                    if((row_ops[0].index1 == col_ops[0].index1)
                       && (row_ops[1].index1 == col_ops[1].index1)
                       && lattice.neighboring(row_ops[2].index1, col_ops[2].index1)) {
                        return J;
                    }
                    return 0.;
                }
            }

            template <typename Prefactor>
            template <typename Term>
            Prefactor MatrixElements<Prefactor>::interaction(const Term& row_term,
                                                             const Term& col_term) const
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");
                const auto len_row = row_term.operators.size();
                const auto len_col = col_term.operators.size();

                assert(valid_1op_term(row_term) || valid_3op_term(row_term));
                assert(valid_1op_term(col_term) || valid_3op_term(col_term));

                const auto& row_ops = row_term.operators;
                const auto& col_ops = col_term.operators;

                if(len_row == 1) {
                    if(len_col == 1) {
                        return (row_ops[0].index1 == col_ops[0].index1) ? U / 2. : 0.;
                    } else {
                        if((row_ops[0].index1 == col_ops[0].index1)
                           && (col_ops[0].index1 == col_ops[1].index1)
                           && (col_ops[1].index1 == col_ops[2].index1))
                            return U / 2.;
                        return 0.;
                    }
                } else {
                    if(len_col == 1) {
                        if((row_ops[0].index1 == col_ops[0].index1)
                           && (row_ops[0].index1 == row_ops[1].index1)
                           && (row_ops[1].index1 == row_ops[2].index1))
                            return U / 2.;
                        return 0.;
                    } else {
                        if((row_ops[0].index1 == col_ops[0].index1)
                           && (row_ops[1].index1 == col_ops[1].index1)
                           && (row_ops[2].index1 == col_ops[2].index1))
                            return U / 2.;
                        return 0.;
                    }
                }
            }
        } /* namespace real_space */

        namespace momentum_space
        {
            template <typename PrefactorT>
            struct MatrixElements {
                using Prefactor = PrefactorT;

                const Prefactor J = 1., U = 1.;

                template <typename Term, typename MomentumSpace, typename Lattice>
                Prefactor hopping(const Term& row_term, const Term& col_term,
                                  const MomentumSpace& momentum_space,
                                  const Lattice& lattice) const;

                template <typename Term, typename MomentumSpace>
                Prefactor interaction(const Term& row_term, const Term& col_term,
                                      const MomentumSpace& momentum_space) const;
            };

            template <typename Prefactor>
            template <typename Term, typename MomentumSpace, typename Lattice>
            Prefactor MatrixElements<Prefactor>::hopping(const Term& row_term, const Term& col_term,
                                                         const MomentumSpace& momentum_space,
                                                         const Lattice& lattice) const
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");
                const auto len_row = row_term.operators.size();
                const auto len_col = col_term.operators.size();

                static const Dispersion<typename MomentumSpace::Vector, Lattice> disp{J};

                assert(valid_1op_term(row_term) || valid_3op_term(row_term));
                assert(valid_1op_term(col_term) || valid_3op_term(col_term));

                if(len_row == 1) {
                    if(len_col == 1) {
                        const auto& op1 = row_term.operators[0];
                        if(op1.same_indices(col_term.operators.front())) {
                            return disp(momentum_space[op1.index1], lattice);
                        }
                    }
                    return 0.;
                } else {
                    if(len_col == 1) {
                        return 0.;
                    } else {
                        const auto& op1 = row_term.operators[0];
                        const auto& op2 = row_term.operators[1];
                        const auto& op3 = row_term.operators[2];
                        if(!op1.same_indices(col_term.operators[0])) return 0.;
                        if(!op2.same_indices(col_term.operators[1])) return 0.;
                        if(!op3.same_indices(col_term.operators[2])) return 0.;
                        return disp(momentum_space[op1.index1], lattice)
                               - disp(momentum_space[op2.index1], lattice)
                               + disp(momentum_space[op3.index1], lattice);
                    }
                }
            }

            template <typename Prefactor>
            template <typename Term, typename MomentumSpace>
            Prefactor
            MatrixElements<Prefactor>::interaction(const Term& row_term, const Term& col_term,
                                                   const MomentumSpace& momentum_space) const
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");
                const auto len_row = row_term.operators.size();
                const auto len_col = col_term.operators.size();

                assert(valid_1op_term(row_term) || valid_3op_term(row_term));
                assert(valid_1op_term(col_term) || valid_3op_term(col_term));

                if(len_row == 1) {
                    if(len_col == 1) {
                        const auto& op1 = row_term.operators[0];
                        if(op1.same_indices(col_term.operators.front())) {
                            return U/2.;
                        }
                        return 0.;
                    } else {
                        const auto& op1 = col_term.operators[0];
                        const auto& op2 = col_term.operators[1];
                        const auto& op3 = col_term.operators[2];
                        auto momentum_total = momentum_space[op1.index1]
                                              + momentum_space[op2.index1]
                                              - momentum_space[op3.index1];
                        if(row_term.operators[0].index1 == momentum_space(momentum_total)) {
                            return U / momentum_space.num();
                        }
                        return 0.;
                    }
                } else {
                    if(len_col == 1) {
                        const auto& op1 = row_term.operators[0];
                        const auto& op2 = row_term.operators[1];
                        const auto& op3 = row_term.operators[2];
                        auto momentum_total = momentum_space[op1.index1]
                                              + momentum_space[op2.index1]
                                              - momentum_space[op3.index1];
                        if(col_term.operators[0].index1 == momentum_space(momentum_total)) {
                            return U / (2 * momentum_space.num());
                        }
                        return 0.;
                    } else {
                        return 0.;
                    }
                }
            }
        } /* namespace momentum_space */
    } /* namespace hubbard */

    template <typename Prefactor>
    struct VariableDescription<hubbard::real_space::MatrixElements<Prefactor>> {
        static Description get(const hubbard::real_space::MatrixElements<Prefactor>& m)
        {
            return {{"MatrixElements (real space)", ""},
                    {"  J", std::to_string(m.J)},
                    {"  U", std::to_string(m.U)}};
        }
    };

    template <typename Prefactor>
    struct VariableDescription<hubbard::momentum_space::MatrixElements<Prefactor>> {
        static Description get(const hubbard::momentum_space::MatrixElements<Prefactor>& m)
        {
            return {{"MatrixElements (momentum space)", ""},
                    {"  J", std::to_string(m.J)},
                    {"  U", std::to_string(m.U)}};
        }
    };
} /* namespace ieompp */


#endif
