#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_BLAZE_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_BLAZE_HPP_

#include <ieompp/models/hubbard/basis.hpp>
#include <ieompp/models/hubbard/operator.hpp>
#include <ieompp/types/blaze.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace detail
        {
            template <typename ScalarT, typename IndexT>
            struct Triplet {
                using Scalar = ScalarT;
                using Index  = IndexT;

                Index row, column;
                Scalar value;

                Triplet(Index i, Index j, Scalar val) : row(i), column(j), value(std::move(val)) {}
            };

            template <typename Scalar, typename Index>
            struct TripletList : public std::vector<Triplet<Scalar, Index>> {
                void sort()
                {
                    std::sort(this->begin(), this->end(),
                              [](const Triplet<Scalar, Index>& a, const Triplet<Scalar, Index>& b) {
                                  return a.column < b.column;
                              });
                }
            };
        } // namespace detail

        namespace real_space
        {
            template <typename Term, typename Lattice>
            uint64_t number_of_kinetic_elements(const Basis1Operator<Term>& basis,
                                                const Lattice& lattice)
            {
                (void)lattice;
                return basis.N * Lattice::coordination_number;
            }

            template <typename Term, typename Lattice>
            uint64_t number_of_kinetic_elements(const Basis3Operator<Term>& basis,
                                                const Lattice& lattice)
            {
                (void)lattice;
                return (basis.N * Lattice::coordination_number)
                       + (basis.N * basis.N_squared * 6 * Lattice::coordination_number);
            }

            template <typename Term>
            uint64_t number_of_interaction_elements(const Basis3Operator<Term>& basis)
            {
                return basis.size() * 2;
            }

            template <typename Matrix, typename Term, typename Lattice, typename Prefactor>
            typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
            init_kinetic_matrix(Matrix& matrix, const Basis1Operator<Term>& basis,
                                const Lattice& lattice, const Prefactor& J = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                using Scalar      = typename types::ScalarType<Matrix>::type;
                using Index       = typename types::IndexType<Matrix>::type;
                using TripletList = detail::TripletList<Scalar, Index>;

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements(basis, lattice));

                TripletList triplets;
                for(Index row = 0; row < basis.N; ++row) {
                    triplets.clear();

                    const auto neighbors = lattice.neighbors(row);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, neighbor, -J);
                    }
                    triplets.sort();

                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }
            }

            template <typename Matrix, typename Term, typename Lattice, typename Prefactor>
            typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
            init_kinetic_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                const Lattice& lattice, const Prefactor& J = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                using Scalar      = typename types::ScalarType<Matrix>::type;
                using Index       = typename types::IndexType<Matrix>::type;
                using TripletList = detail::TripletList<Scalar, Index>;

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements(basis, lattice));

                TripletList triplets;
                for(Index row = 0; row < basis.N; ++row) {
                    triplets.clear();

                    const auto neighbors = lattice.neighbors(row);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, neighbor, -J);
                    }
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
                            row, basis.get_3op_index(neighbor, ops[1].index1, ops[2].index1), -J);
                    }

                    neighbors = lattice.neighbors(ops[1].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(ops[0].index1, neighbor, ops[2].index1), -J);
                    }

                    neighbors = lattice.neighbors(ops[2].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(ops[0].index1, ops[1].index1, neighbor), J);
                    }

                    triplets.sort();
                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }
            }

            template <typename Matrix, typename Term, typename Prefactor>
            typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
            init_interaction_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                    const Prefactor& U = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                using Index = typename types::IndexType<Matrix>::type;

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_interaction_elements(basis));

                for(Index row = 0; row < basis.N; ++row) {
                    matrix.append(row, row, U / 2.);
                    matrix.append(row, basis.get_3op_index(row, row, row), U / 2.);
                    matrix.finalize(row);
                }

                for(Index row = basis.N; row < basis.size(); ++row) {
                    const auto& ops = basis[row].operators;
                    if((ops[0].index1 == ops[1].index1) && (ops[0].index1 == ops[2].index1)) {
                        matrix.append(row, ops[0].index1, U / 2.);
                    }
                    matrix.append(row, row, U / 2.);
                    matrix.finalize(row);
                }
            }

            template <typename Matrix, typename Term, typename Lattice, typename Prefactor>
            typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
            init_matrix(Matrix& matrix, const Basis3Operator<Term>& basis, const Lattice& lattice,
                        const Prefactor& J = 1., const Prefactor& U = 1.)
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                using Scalar      = typename types::ScalarType<Matrix>::type;
                using Index       = typename types::IndexType<Matrix>::type;
                using TripletList = detail::TripletList<Scalar, Index>;

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_kinetic_elements(basis, lattice)
                               + number_of_interaction_elements(basis));

                TripletList triplets;
                for(Index row = 0; row < basis.N; ++row) {
                    triplets.clear();

                    auto neighbors = lattice.neighbors(row);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(row, neighbor, -J);
                    }
                    triplets.emplace_back(row, row, U / 2.);
                    triplets.emplace_back(row, basis.get_3op_index(row, row, row), U / 2.);

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
                            row, basis.get_3op_index(neighbor, ops[1].index1, ops[2].index1), -J);
                    }

                    neighbors = lattice.neighbors(ops[1].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(ops[0].index1, neighbor, ops[2].index1), -J);
                    }

                    neighbors = lattice.neighbors(ops[2].index1);
                    for(auto neighbor : neighbors) {
                        triplets.emplace_back(
                            row, basis.get_3op_index(ops[0].index1, ops[1].index1, neighbor), J);
                    }

                    if((ops[0].index1 == ops[1].index1) && (ops[0].index1 == ops[2].index1)) {
                        triplets.emplace_back(row, ops[0].index1, U / 2.);
                    }
                    triplets.emplace_back(row, row, U / 2.);

                    triplets.sort();
                    for(const auto triplet : triplets) {
                        matrix.append(row, triplet.column, triplet.value);
                    }
                    matrix.finalize(row);
                }
            }
        } /* namespace real_space */
    }     /* namespace hubbard */
} /* namespace ieompp */

#endif
