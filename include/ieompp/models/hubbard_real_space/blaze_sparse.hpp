#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_BLAZE_SPARSE_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_BLAZE_SPARSE_HPP_

#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/models/hubbard_real_space/liouvillian.hpp"
#include "ieompp/types/triplet.hpp"

#include <cstdint>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Monomial, typename Lattice>
            uint64_t number_of_kinetic_elements(const Basis1Operator<Monomial>& basis)
            {
                return basis.N * Lattice::coordination_number;
            }

            template <typename Monomial, typename Lattice>
            uint64_t number_of_kinetic_elements(const Basis3Operator<Monomial>& basis)
            {
                return (basis.N * Lattice::coordination_number)
                       + (basis.N * basis.N_squared * 6 * Lattice::coordination_number);
            }

            template <typename Monomial>
            uint64_t number_of_interaction_elements(const Basis3Operator<Monomial>& basis)
            {
                return basis.size() * 2;
            }

            template <typename Liouvillian, typename Matrix, typename Monomial, typename Lattice>
            void init_kinetic_matrix(const Liouvillian& liouvillian, Matrix& matrix,
                                     const Basis1Operator<Monomial>& basis, const Lattice& lattice)
            {
                using Scalar = typename types::ScalarType<Matrix>::Type;
                using Index  = typename types::IndexType<Matrix>::Type;

                static_assert(hubbard::IsHubbardOperator<typename Monomial::Operator>::value,
                              "Operator-type in Monomial-type must be a Hubbard like operator!");

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements<Monomial, Lattice>(basis));

                types::TripletList<Scalar, Index> triplets;
                for(Index row = 0; row < basis.N; ++row) {
                    triplets.clear();

                    const auto neighbors = lattice.neighbors(row);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, neighbor, -liouvillian.J);
                    }
                    triplets.sort();

                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }
            }

            template <typename Liouvillian, typename Matrix, typename Monomial, typename Lattice>
            void init_matrix(const Liouvillian& liouvillian, Matrix& matrix,
                             const Basis3Operator<Monomial>& basis, const Lattice& lattice)
            {
                using Scalar = typename types::ScalarType<Matrix>::Type;
                using Index  = typename types::IndexType<Matrix>::Type;

                static_assert(hubbard::IsHubbardOperator<typename Monomial::Operator>::value,
                              "Operator-type in Monomial-type must be a Hubbard like operator!");

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements<Monomial, Lattice>(basis)
                               + number_of_interaction_elements(basis));

                types::TripletList<Scalar, Index> triplets;
                for(Index row = 0; row < basis.N; ++row) {
                    triplets.clear();

                    auto neighbors = lattice.neighbors(row);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, neighbor, -liouvillian.J);
                    }
                    triplets.emplace_back(row, row, liouvillian.U / 2.);
                    triplets.emplace_back(row, basis.get_3op_index(row, row, row),
                                          liouvillian.U / 2.);

                    triplets.sort();
                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }

                for(Index row = basis.N; row < basis.size(); ++row) {
                    triplets.clear();

                    const auto& monomial = basis[row];
                    auto neighbors       = lattice.neighbors(monomial[0].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, basis.get_3op_index(neighbor, monomial[1].index1,
                                                                       monomial[2].index1),
                                              -liouvillian.J);
                    }

                    neighbors = lattice.neighbors(monomial[1].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, basis.get_3op_index(monomial[0].index1, neighbor,
                                                                       monomial[2].index1),
                                              -liouvillian.J);
                    }

                    neighbors = lattice.neighbors(monomial[2].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row,
                            basis.get_3op_index(monomial[0].index1, monomial[1].index1, neighbor),
                            liouvillian.J);
                    }

                    if((monomial[0].index1 == monomial[1].index1)
                       && (monomial[0].index1 == monomial[2].index1)) {
                        triplets.emplace_back(row, monomial[0].index1, liouvillian.U / 2.);
                    }
                    triplets.emplace_back(row, row, liouvillian.U / 2.);

                    triplets.sort();
                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }
            }
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
