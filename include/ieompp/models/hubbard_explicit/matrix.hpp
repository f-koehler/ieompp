#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_HPP_

#include <type_traits>

#include <ieompp/types/eigen.hpp>
#include <ieompp/models/hubbard_explicit/operator.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Basis, typename Container, typename Lattice>
            typename std::
                enable_if<ieompp::types::is_eigen_triplet<typename Container::value_type>::value,
                          void>::type
                init_kinetic_matrix(Container& matrix, const Basis& basis, Lattice& lattice,
                                    const typename ieompp::types::is_eigen_triplet<
                                        typename Container::value_type>::Scalar& J = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Basis::Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                using Index = typename Basis::Index;
                const auto basis_size = basis.size();

                for(Index i = 0; i < basis.N; ++i) {
                    const auto neighbors = lattice.unique_neighbors(i);
                    for(auto neighbor : neighbors) matrix.emplace_back(i, neighbor, -J);
                }

                for(Index i = basis.N; i < basis_size; ++i) {
                    const auto& ops = basis[i].operators;

                    auto neighbors = lattice.unique_neighbors(ops[0].index1);
                    for(auto neighbor : neighbors) {
                        matrix.emplace_back(
                            i, basis.get_3op_index(neighbor, ops[1].index1, ops[2].index1), -J);
                    }

                    neighbors = lattice.unique_neighbors(ops[1].index1);
                    for(auto neighbor : neighbors) {
                        matrix.emplace_back(
                            i, basis.get_3op_index(ops[0].index1, neighbor, ops[2].index1), -J);
                    }

                    neighbors = lattice.unique_neighbors(ops[2].index1);
                    for(auto neighbor : neighbors) {
                        matrix.emplace_back(
                            i, basis.get_3op_index(ops[0].index1, ops[2].index1, neighbor), J);
                    }
                }
            }

            template <typename Basis, typename Container>
            typename std::
                enable_if<ieompp::types::is_eigen_triplet<typename Container::value_type>::value,
                          void>::type
                init_interaction_matrix(Container& matrix, const Basis& basis,
                                        const typename ieompp::types::is_eigen_triplet<
                                            typename Container::value_type>::Scalar& U = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Basis::Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                using Index = typename Basis::Index;
                const auto basis_size = basis.size();

                for(Index i = 0; i < basis.N; ++i) {
                    matrix.emplace_back(i, i, U / 2.);
                    matrix.emplace_back(i, basis.get_3op_index(i, i, i), U / 2.);
                }

                for(Index i = basis.N; i < basis_size; ++i) {
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
