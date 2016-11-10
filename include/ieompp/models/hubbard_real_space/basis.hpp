#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_BASIS_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_BASIS_HPP_

#include <vector>

#include "ieompp/models/hubbard_common/operator_traits.hpp"

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename TermT>
            struct Basis1Operator : public std::vector<TermT> {
                using Term  = TermT;
                using Index = typename std::vector<Term>::size_type;

                const Index N;

                template <typename Lattice>
                Basis1Operator(const Lattice& lattice) : N(lattice.num())
                {
                    static_assert(
                        hubbard_common::is_hubbard_operator<typename Term::Operator>::value,
                        "Operator must be of Hubbard type");

                    this->reserve(N);

                    for(auto i : lattice) {
                        this->push_back(Term{1, {{true, i, true}}});
                    }
                }
            };

            template <typename TermT>
            struct Basis3Operator : public std::vector<TermT> {
                using Term  = TermT;
                using Index = typename std::vector<Term>::size_type;

                const Index N;
                const Index N_squared;

                template <typename Lattice>
                Basis3Operator(const Lattice& lattice) : N(lattice.num()), N_squared(N * N)
                {
                    static_assert(
                        hubbard_common::is_hubbard_operator<typename Term::Operator>::value,
                        "Operator must be of Hubbard type");

                    this->reserve(N * (N * N + 1));
                    const auto prefactor = typename Term::Prefactor(1.);

                    for(auto i : lattice) {
                        this->push_back(Term{prefactor, {{true, i, true}}});
                    }
                    for(auto i1 : lattice) {
                        for(auto i2 : lattice) {
                            for(auto i3 : lattice) {
                                this->push_back(Term{
                                    prefactor,
                                    {{true, i1, true}, {true, i2, false}, {false, i3, false}}});
                            }
                        }
                    }
                }

                Index get_3op_index(Index i1, Index i2, Index i3) const
                {
                    return N + N_squared * i1 + N * i2 + i3;
                }
            };

            template <typename Basis>
            struct IsOneOperatorBasis {
                static constexpr bool value = false;
            };

            template <typename Basis>
            struct IsThreeOperatorBasis {
                static constexpr bool value = false;
            };

            template <typename Term>
            struct IsOneOperatorBasis<Basis1Operator<Term>> {
                static constexpr bool value = true;
            };

            template <typename Term>
            struct IsThreeOperatorBasis<Basis3Operator<Term>> {
                static constexpr bool value = true;
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
