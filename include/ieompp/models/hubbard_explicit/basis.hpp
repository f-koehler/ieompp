#ifndef IEOMPP_MODELS_HUBBARD_BASIS_HPP_
#define IEOMPP_MODELS_HUBBARD_BASIS_HPP_

#include <vector>

#include <ieompp/models/hubbard_explicit/operator.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename TermT>
            struct Basis1Operator : public std::vector<TermT> {
                using Term  = TermT;
                using Index = typename std::vector<Term>::size_type;

                static_assert(is_hubbard_operator<typename Term::Operator>::value,
                              "Operator must be of Hubbard type");

                const Index N;

                template <typename Lattice>
                Basis1Operator(const Lattice& lattice) : N(lattice.num())
                {
                    this->reserve(N);
                    for(auto i : lattice) this->emplace_back(Term{1, {{true, i, true}}});
                }
            };

            template <typename TermT>
            struct Basis3Operator : public std::vector<TermT> {
                using Term  = TermT;
                using Index = typename std::vector<Term>::size_type;

                static_assert(is_hubbard_operator<typename Term::Operator>::value,
                              "Operator must be of Hubbard type");

                const Index N;
                const Index N_squared;

                template <typename Lattice>
                Basis3Operator(const Lattice& lattice) : N(lattice.num()), N_squared(N * N)
                {
                    // TODO: emplace
                    this->reserve(N * (N * N + 1));

                    for(auto i : lattice) this->push_back(Term{1, {{true, i, true}}});
                    for(auto i1 : lattice) {
                        for(auto i2 : lattice) {
                            for(auto i3 : lattice)
                                this->push_back(Term{
                                    1, {{true, i1, true}, {true, i2, false}, {false, i3, false}}});
                        }
                    }
                }

                Index get_3op_index(Index i1, Index i2, Index i3) const
                {
                    return N + N_squared * i1 + N * i2 + i3;
                }
            };
        } /* namespace real_space */

        namespace momentum_space
        {
            template <typename TermT>
            struct Basis3Operator : public std::vector<TermT> {
                using Term  = TermT;
                using Index = typename std::vector<Term>::size_type;

                static_assert(is_hubbard_operator<typename Term::Operator>::value,
                              "Operator must be of Hubbard type");

                const std::size_t N;

                template <typename MomentumSpace>
                Basis3Operator(Index q_idx, const MomentumSpace& momentum_space)
                    : N(momentum_space.num())
                {
                    // TODO: emplace?
                    this->reserve(N * N + 1);

                    for(auto i : momentum_space) this->push_back(Term{1, {{true, q_idx, true}}});

                    const auto q = momentum_space[q_idx];
                    for(auto i1 : momentum_space) {
                        const auto k1 = momentum_space[i1];
                        for(auto i2 : momentum_space) {
                            auto i3 = momentum_space(k1 + momentum_space[i2] - q);
                            this->push_back(
                                Term{1, {{true, i1, true}, {true, i2, false}, {false, i3, false}}});
                        }
                    }
                }
            };
        } /* namespace momentum_space */
    } /* namespace hubbard */
} /* namespace ieompp */

#endif
