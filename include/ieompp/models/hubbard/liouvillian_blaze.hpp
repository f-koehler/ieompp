#ifndef IEOMPP_HUBBARD_LIOUVILLIAN_BLAZE_HPP_
#define IEOMPP_HUBBARD_LIOUVILLIAN_BLAZE_HPP_

#include <algorithm>
#include <vector>

#include "ieompp/models/hubbard/basis.hpp"
#include "ieompp/models/hubbard/dispersion.hpp"
#include "ieompp/types/blaze/traits.hpp"

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename PrefactorT>
            struct Liouvillian {
                using Prefactor = PrefactorT;

                Prefactor J = 1.;
                Prefactor U = 1.;

                template <typename Term, typename Lattice>
                static uint64_t number_of_kinetic_elements(const Basis1Operator<Term>& basis,
                                                           const Lattice& lattice);

                template <typename Term, typename Lattice>
                static uint64_t number_of_kinetic_elements(const Basis3Operator<Term>& basis,
                                                           const Lattice& lattice);

                template <typename Term>
                static uint64_t number_of_interaction_elements(const Basis3Operator<Term>& basis);


                template <typename Matrix, typename Term, typename Lattice>
                void init_kinetic_matrix(Matrix& matrix, const Basis1Operator<Term>& basis,
                                         const Lattice& lattice) const;

                template <typename Matrix, typename Term, typename Lattice>
                void init_kinetic_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                         const Lattice& lattice) const;

                template <typename Matrix, typename Term>
                void init_interaction_matrix(Matrix& matrix,
                                             const Basis3Operator<Term>& basis) const;

                template <typename Matrix, typename Term, typename Lattice>
                void init_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                 const Lattice& lattice) const;
            };
        } // namespace real_space

        namespace momentum_space
        {
            template <typename PrefactorT>
            struct Liouvillian {
                using Prefactor = PrefactorT;

                Prefactor U = 1.;

                template <typename Term>
                static uint64_t number_of_matrix_elements(const Basis3Operator<Term>& basis);

                template <typename Matrix, typename Term, typename Lattice>
                void init_matrix(Matrix& matrix, const Basis3Operator<Term>& basis,
                                 const Lattice& momentum_space, const Lattice& lattice,
                                 const Dispersion<Lattice>& dispersion) const;
            };
        } // namespace momentum_space

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
            template <typename Prefactor>
            template <typename Term, typename Lattice>
            uint64_t Liouvillian<Prefactor>::number_of_kinetic_elements(
                const real_space::Basis1Operator<Term>& basis, const Lattice& lattice)
            {
                static_cast<void>(lattice);
                return basis.N * Lattice::coordination_number;
            }

            template <typename Prefactor>
            template <typename Term, typename Lattice>
            uint64_t Liouvillian<Prefactor>::number_of_kinetic_elements(
                const real_space::Basis3Operator<Term>& basis, const Lattice& lattice)
            {
                static_cast<void>(lattice);
                return (basis.N * Lattice::coordination_number)
                       + (basis.N * basis.N_squared * 6 * Lattice::coordination_number);
            }

            template <typename Prefactor>
            template <typename Term>
            uint64_t Liouvillian<Prefactor>::number_of_interaction_elements(
                const real_space::Basis3Operator<Term>& basis)
            {
                return basis.size() * 2;
            }


            template <typename Prefactor>
            template <typename Matrix, typename Term, typename Lattice>
            void Liouvillian<Prefactor>::init_kinetic_matrix(Matrix& matrix,
                                                             const Basis1Operator<Term>& basis,
                                                             const Lattice& lattice) const
            {
                using Scalar      = typename types::ScalarType<Matrix>::type;
                using Index       = typename types::IndexType<Matrix>::type;
                using TripletList = detail::TripletList<Scalar, Index>;

                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

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

            template <typename Prefactor>
            template <typename Matrix, typename Term, typename Lattice>
            void Liouvillian<Prefactor>::init_kinetic_matrix(Matrix& matrix,
                                                             const Basis3Operator<Term>& basis,
                                                             const Lattice& lattice) const
            {
                using Scalar      = typename types::ScalarType<Matrix>::type;
                using Index       = typename types::IndexType<Matrix>::type;
                using TripletList = detail::TripletList<Scalar, Index>;

                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

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

            template <typename Prefactor>
            template <typename Matrix, typename Term>
            void Liouvillian<Prefactor>::init_interaction_matrix(
                Matrix& matrix, const real_space::Basis3Operator<Term>& basis) const
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

            template <typename Prefactor>
            template <typename Matrix, typename Term, typename Lattice>
            void Liouvillian<Prefactor>::init_matrix(Matrix& matrix,
                                                     const real_space::Basis3Operator<Term>& basis,
                                                     const Lattice& lattice) const
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
        } // namespace real_space

        namespace momentum_space
        {
            template <typename Prefactor>
            template <typename Term>
            uint64_t Liouvillian<Prefactor>::number_of_matrix_elements(
                const momentum_space::Basis3Operator<Term>& basis)
            {
                return basis.N * (1 + 3 * basis.N);
            }

            template <typename Prefactor>
            template <typename Matrix, typename Term, typename Lattice>
            void Liouvillian<Prefactor>::init_matrix(Matrix& matrix,
                                                     const Basis3Operator<Term>& basis,
                                                     const Lattice& momentum_space,
                                                     const Lattice& lattice,
                                                     const Dispersion<Lattice>& dispersion) const
            {
                static_assert(ieompp::hubbard::is_hubbard_operator<typename Term::Operator>::value,
                              "Operator-type in Term-type must be a Hubbard like operator!");

                using Scalar = typename types::ScalarType<Matrix>::type;
                using Index  = typename types::IndexType<Matrix>::type;

                const auto interaction1 = U / 2., interaction2 = U / (2. * lattice.num());

                matrix.resize(basis.size(), basis.size(), false);
                matrix.reset();
                matrix.reserve(number_of_matrix_elements(basis));

                const auto& op = basis[0].operators.front();

                // add diagonal element
                matrix.append(0, 0, dispersion[op.index1] + interaction1);

                // add off-diagonal elements
                for(Index i = 1; i < basis.size(); ++i) {
                    matrix.append(0, i, interaction2);
                }
                matrix.finalize(0);

                for(Index row = 1; row < basis.size(); ++row) {
                    const auto& ops = basis[row].operators;

                    // add off-diagonal element
                    matrix.append(row, 0, interaction2);

                    // add diagonal element
                    matrix.append(row, row, dispersion[ops[0].index1] + dispersion[ops[1].index1]
                                                - dispersion[ops[2].index1] + interaction1);
                    matrix.finalize(row);
                }
            }
        } // namespace momentum_space
    }     // namespace hubbard
} // namespace ieompp

#endif
