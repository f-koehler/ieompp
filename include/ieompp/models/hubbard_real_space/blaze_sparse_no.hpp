#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_BLAZE_SPARSE_NO_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_BLAZE_SPARSE_NO_HPP_

#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/models/hubbard_real_space/liouvillian.hpp"
#include "ieompp/types/number.hpp"
#include "ieompp/types/triplet.hpp"

#include <cstdint>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Monomial, typename Lattice>
            uint64_t number_of_kinetic_elements_no(const Basis1Operator<Monomial>& basis)
            {
                return basis.N * Lattice::coordination_number;
            }

            template <typename Monomial, typename Lattice>
            uint64_t number_of_kinetic_elements_no(const Basis3Operator<Monomial>& basis)
            {
                return (basis.N * Lattice::coordination_number)
                       + ((basis.size() - basis.N) * 6 * Lattice::coordination_number);
            }

            template <typename Monomial>
            uint64_t number_of_interaction_elements_no(const Basis3Operator<Monomial>& basis)
            {
                return (basis.N * 2) + ((basis.size() - basis.N) * 8);
            }

            template <typename Liouvillian, typename Matrix, typename Monomial, typename Lattice>
            void init_kinetic_matrix_no(const Liouvillian& liouvillian, Matrix& matrix,
                                        const Basis1Operator<Monomial>& basis,
                                        const Lattice& lattice)
            {
                using Scalar = typename types::ScalarType<Matrix>::Type;
                using Index  = typename types::IndexType<Matrix>::Type;

                static_assert(hubbard_common::IsHubbardOperator<typename Monomial::Operator>::value,
                              "Operator-type in Monomial-type must be a Hubbard like operator!");

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements_no<Monomial, Lattice>(basis));

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

            template <typename Liouvillian, typename Matrix, typename Monomial, typename Lattice,
                      typename ExpectationValueFunction>
            void init_matrix_no(const Liouvillian& liouvillian, Matrix& matrix,
                                const Basis3Operator<Monomial>& basis, const Lattice& lattice,
                                const ExpectationValueFunction& expectation_value)
            {
                using Scalar = typename types::ScalarType<Matrix>::Type;
                using Index  = typename types::IndexType<Matrix>::Type;

                static_assert(hubbard_common::IsHubbardOperator<typename Monomial::Operator>::value,
                              "Operator-type in Monomial-type must be a Hubbard like operator!");

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements_no<Monomial, Lattice>(basis)
                               + number_of_interaction_elements_no(basis));

                types::TripletList<Scalar, Index> triplets;
                for(Index row = 0; row < basis.N; ++row) {
                    triplets.clear();

                    auto neighbors = lattice.neighbors(row);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, neighbor, -liouvillian.J);
                    }

                    // clang-format off
                    triplets.emplace_back(row, row, liouvillian.U);
                    triplets.emplace_back(row, basis.get_3op_index(row, row, row), liouvillian.U * expectation_value(basis[row].front().index1, basis[row].front().index1));
                    // clang-format on

                    triplets.sort();
                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }

                triplets.clear();
                triplets.reserve((Lattice::coordination_number * 3) + 8);
                for(Index row = basis.N; row < basis.size(); ++row) {
                    triplets.clear();

                    const auto& monomial = basis[row];
                    const auto& op1      = monomial[0];
                    const auto& op2      = monomial[1];
                    const auto& op3      = monomial[2];
                    const auto& r1       = op1.index1;
                    const auto& r2       = op2.index1;
                    const auto& r3       = op3.index1;

                    auto neighbors = lattice.neighbors(r1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, basis.get_3op_index(neighbor, r2, r3),
                                              -liouvillian.J);
                    }

                    neighbors = lattice.neighbors(r2);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, basis.get_3op_index(r1, neighbor, r3),
                                              -liouvillian.J);
                    }

                    neighbors = lattice.neighbors(r3);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, basis.get_3op_index(r1, r2, neighbor),
                                              liouvillian.J);
                    }

                    const auto expectation_value_conj = [&expectation_value](const Index& a,
                                                                             const Index& b) {
                        return ((a == b) ? 1 : 0) - expectation_value(b, a);
                    };

                    // 1-operator monomials
                    // clang-format off
                    triplets.emplace_back(row, r1, liouvillian.U * expectation_value(op2.index1, op1.index1) * expectation_value_conj(op3.index1, op1.index1));
                    triplets.emplace_back(row, r2, liouvillian.U * expectation_value_conj(op2.index1, op1.index1) * expectation_value(op2.index1, op3.index1));
                    triplets.emplace_back(row, r3, -liouvillian.U * expectation_value(op3.index1, op1.index1) * expectation_value(op2.index1, op3.index1));
                    // clang-format on

                    // 3-operator monomials
                    // clang-format off
                    triplets.emplace_back(row, row, liouvillian.U * expectation_value(op1.index1, op1.index1));
                    triplets.emplace_back(row, basis.get_3op_index(r1, r1, r3), -liouvillian.U * expectation_value(op2.index1, op1.index1));
                    triplets.emplace_back(row, basis.get_3op_index(r1, r2, r1), liouvillian.U * expectation_value_conj(op3.index1, op1.index1));
                    if(r2 != r3) {
                        triplets.emplace_back(row, basis.get_3op_index(r2, r2, r3), liouvillian.U * expectation_value(op2.index1, op1.index1));
                        triplets.emplace_back(row, basis.get_3op_index(r3, r2, r3), -liouvillian.U * expectation_value(op3.index1, op1.index1));
                    }
                    // clang-format on

                    // triplet list may contain triplets for the same column
                    triplets.sort();
                    triplets = triplets.make_columns_unique();

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
