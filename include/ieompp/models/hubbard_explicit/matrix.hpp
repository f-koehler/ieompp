#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_HPP_

#include <type_traits>

#include <ieompp/types/eigen_triplet_list.hpp>
#include <ieompp/models/hubbard_explicit/operator.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Scalar, typename Index, typename Basis, typename Lattice>
            void init_kinetic_matrix(types::TripletList<Scalar, Index>& matrix, const Basis& basis,
                                     Lattice& lattice, const Scalar& J = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Basis::Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                const auto basis_size = basis.size();

                for(typename Basis::Index i = 0; i < basis.N; ++i) {
                    const auto neighbors = lattice.unique_neighbors(i);
                    for(auto neighbor : neighbors) matrix.emplace_back(i, neighbor, -J);
                }

                for(typename Basis::Index i = basis.N; i < basis_size; ++i) {
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

            template <typename Scalar, typename Index, typename Basis>
            void init_interaction_matrix(types::TripletList<Scalar, Index>& matrix,
                                         const Basis& basis, const Scalar& U = 1.)
            {
                static_assert(
                    ieompp::hubbard::is_hubbard_operator<typename Basis::Term::Operator>::value,
                    "Operator-type in Term-type must be a Hubbard like operator!");

                const auto basis_size = basis.size();

                for(typename Basis::Index i = 0; i < basis.N; ++i) {
                    matrix.emplace_back(i, i, U / 2.);
                    matrix.emplace_back(i, basis.get_3op_index(i, i, i), U / 2.);
                }

                for(typename Basis::Index i = basis.N; i < basis_size; ++i) {
                    matrix.emplace_back(i, i, U / 2.);
                    const auto& ops = basis[i].operators;
                    if((ops[0].index1 == ops[1].index1) && (ops[0].index1 == ops[2].index1))
                        matrix.emplace_back(i, ops[0].index1, U / 2.);
                }
            }

            template <typename Basis, typename Container>
            void sort(Container& container, bool col_major = true)
            {
                if(col_major) {
                    std::sort(container.begin(), container.end(),
                              [](const Eigen::Triplet<double>& a, const Eigen::Triplet<double>& b) {
                                  return (a.col() < b.col()) || (a.row() < b.row());
                              });
                } else {
                    std::sort(container.begin(), container.end(),
                              [](const Eigen::Triplet<double>& a, const Eigen::Triplet<double>& b) {
                                  return (a.row() < b.row()) || (a.col() < b.col());
                              });
                }
            }
        }
    }
}

#endif
