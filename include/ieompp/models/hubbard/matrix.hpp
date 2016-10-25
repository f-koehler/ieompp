#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_HPP_

#include <type_traits>

#include "ieompp/models/hubbard/basis.hpp"
#include "ieompp/models/hubbard/operator.hpp"
#include "ieompp/types/matrix.hpp"

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Matrix, typename Term, typename Lattice>
            void init_kinetic_matrix(Matrix& matrix, const Basis1Operator<Term>& basis,
                                     const Lattice& lattice,
                                     const typename types::ScalarType<Matrix>::type& J = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                for(typename Basis1Operator<Term>::Index i = 0; i < basis.N; ++i) {
                    const auto neighbors = lattice.neighbors(i);
                    for(auto neighbor : neighbors) {
                        types::matrix_insert(matrix, i, neighbor, -J);
                    }
                }
            }

            template <typename Matrix, typename Term, typename Lattice>
            void init_kinetic_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                     const Lattice& lattice,
                                     const typename types::ScalarType<Matrix>::type& J = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                const auto basis_size = basis.size();

                for(typename Basis3Operator<Term>::Index i = 0; i < basis.N; ++i) {
                    const auto neighbors = lattice.neighbors(i);
                    for(auto neighbor : neighbors) {
                        types::matrix_insert(matrix, i, neighbor, -J);
                    }
                }

                for(typename Basis3Operator<Term>::Index i = basis.N; i < basis_size; ++i) {
                    const auto& ops = basis[i].operators;

                    auto neighbors = lattice.neighbors(ops[0].index1);
                    for(auto neighbor : neighbors) {
                        types::matrix_insert(
                            matrix, i, basis.get_3op_index(neighbor, ops[1].index1, ops[2].index1),
                            -J);
                    }

                    neighbors = lattice.neighbors(ops[1].index1);
                    for(auto neighbor : neighbors) {
                        types::matrix_insert(
                            matrix, i, basis.get_3op_index(ops[0].index1, neighbor, ops[2].index1),
                            -J);
                    }

                    neighbors = lattice.neighbors(ops[2].index1);
                    for(auto neighbor : neighbors) {
                        types::matrix_insert(
                            matrix, i, basis.get_3op_index(ops[0].index1, ops[1].index1, neighbor),
                            J);
                    }
                }
            }

            template <typename Matrix, typename Term>
            void init_interaction_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                         const typename types::ScalarType<Matrix>::type& U = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                const auto basis_size = basis.size();

                for(typename Basis3Operator<Term>::Index i = 0; i < basis.N; ++i) {
                    types::matrix_insert(matrix, i, i, U / 2.);
                    types::matrix_insert(matrix, i, basis.get_3op_index(i, i, i), U / 2.);
                }

                for(typename Basis3Operator<Term>::Index i = basis.N; i < basis_size; ++i) {
                    types::matrix_insert(matrix, i, i, U / 2.);
                    const auto& ops = basis[i].operators;
                    if((ops[0].index1 == ops[1].index1) && (ops[0].index1 == ops[2].index1)) {
                        types::matrix_insert(matrix, i, ops[0].index1, U / 2.);
                    }
                }
            }
        } // namespace real_space
    }     // namespace hubbard
} // namespace ieompp

#endif
