#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_HPP_

#include <type_traits>

#include <ieompp/types/matrix.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/models/hubbard_explicit/operator.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Matrix, typename Term, typename Lattice>
            void init_kinetic_matrix_1(Matrix& matrix, const Basis1Operator<Term>& basis,
                                       Lattice& lattice,
                                       const typename types::scalar_type<Matrix>::type& J = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                for(typename Basis1Operator<Term>::Index i = 0; i < basis.N; ++i) {
                    const auto neighbors = lattice.neighbors(i);
                    for(auto neighbor : neighbors) matrix.emplace_back(i, neighbor, -J);
                }
            }

            template <typename Matrix, typename Term, typename Lattice>
            void init_kinetic_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                     Lattice& lattice,
                                     const typename types::scalar_type<Matrix>::type& J = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                const auto basis_size = basis.size();

                for(typename Basis3Operator<Term>::Index i = 0; i < basis.N; ++i) {
                    const auto neighbors = lattice.neighbors(i);
                    for(auto neighbor : neighbors) matrix.emplace_back(i, neighbor, -J);
                }

                for(typename Basis3Operator<Term>::Index i = basis.N; i < basis_size; ++i) {
                    const auto& ops = basis[i].operators;

                    auto neighbors = lattice.neighbors(ops[0].index1);
                    for(auto neighbor : neighbors) {
                        matrix.emplace_back(
                            i, basis.get_3op_index(neighbor, ops[1].index1, ops[2].index1), -J);
                    }

                    neighbors = lattice.neighbors(ops[1].index1);
                    for(auto neighbor : neighbors) {
                        matrix.emplace_back(
                            i, basis.get_3op_index(ops[0].index1, neighbor, ops[2].index1), -J);
                    }

                    neighbors = lattice.neighbors(ops[2].index1);
                    for(auto neighbor : neighbors) {
                        matrix.emplace_back(
                            i, basis.get_3op_index(ops[0].index1, ops[1].index1, neighbor), J);
                    }
                }
            }

            template <typename Matrix, typename Term>
            void init_interaction_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                         const typename types::scalar_type<Matrix>::type& U = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                const auto basis_size = basis.size();

                for(typename Basis3Operator<Term>::Index i = 0; i < basis.N; ++i) {
                    matrix.emplace_back(i, i, U / 2.);
                    matrix.emplace_back(i, basis.get_3op_index(i, i, i), U / 2.);
                }

                for(typename Basis3Operator<Term>::Index i = basis.N; i < basis_size; ++i) {
                    matrix.emplace_back(i, i, U / 2.);
                    const auto& ops = basis[i].operators;
                    if((ops[0].index1 == ops[1].index1) && (ops[0].index1 == ops[2].index1))
                        matrix.emplace_back(i, ops[0].index1, U / 2.);
                }
            }
        }
    }
}

#endif
