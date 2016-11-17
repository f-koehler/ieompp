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
            template <typename Term, typename Lattice>
            uint64_t number_of_kinetic_elements(const Basis1Operator<Term>& basis)
            {
                return basis.N * Lattice::coordination_number;
            }

            template <typename Term, typename Lattice>
            uint64_t number_of_kinetic_elements(const Basis3Operator<Term>& basis)
            {
                return (basis.N * Lattice::coordination_number)
                       + (basis.N * basis.N_squared * 6 * Lattice::coordination_number);
            }

            template <typename Term>
            uint64_t number_of_interaction_elements(const Basis3Operator<Term>& basis)
            {
                return basis.size() * 2;
            }

            template <typename Liouvillian, typename Matrix, typename Term, typename Lattice>
            void init_kinetic_matrix(const Liouvillian& liouvillian, Matrix& matrix,
                                     const Basis1Operator<Term>& basis, const Lattice& lattice)
            {
                using Scalar = typename types::ScalarType<Matrix>::Type;
                using Index  = typename types::IndexType<Matrix>::Type;

                static_assert(hubbard_common::IsHubbardOperator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements<Term, Lattice>(basis));

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

            template <typename Liouvillian, typename Matrix, typename Term, typename Lattice>
            void init_matrix(const Liouvillian& liouvillian, Matrix& matrix,
                             const Basis3Operator<Term>& basis, const Lattice& lattice)
            {
                static_assert(hubbard_common::IsHubbardOperator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                using Scalar = typename types::ScalarType<Matrix>::Type;
                using Index  = typename types::IndexType<Matrix>::Type;
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements<Term, Lattice>(basis)
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

                    const auto& ops = basis[row].operators;
                    auto neighbors  = lattice.neighbors(ops[0].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(neighbor, ops[1].index1, ops[2].index1),
                            -liouvillian.J);
                    }

                    neighbors = lattice.neighbors(ops[1].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(ops[0].index1, neighbor, ops[2].index1),
                            -liouvillian.J);
                    }

                    neighbors = lattice.neighbors(ops[2].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(ops[0].index1, ops[1].index1, neighbor),
                            liouvillian.J);
                    }

                    if((ops[0].index1 == ops[1].index1) && (ops[0].index1 == ops[2].index1)) {
                        triplets.emplace_back(row, ops[0].index1, liouvillian.U / 2.);
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
