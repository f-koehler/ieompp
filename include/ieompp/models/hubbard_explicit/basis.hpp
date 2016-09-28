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
            struct Basis3Operator : public std::vector<TermT> {
                using Term  = TermT;
                using Index = typename std::vector<Term>::size_type;

                static_assert(is_hubbard_operator<typename Term::Operator>::value,
                              "Operator must be of Hubbard type");

                template <typename Lattice>
                Basis3Operator(const Lattice& lattice)
                {
                    // TODO: emplace
                    const auto N = lattice.num();
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

                template <typename Lattice>
                Index operator()(const Term& t, const Lattice& lattice) const
                {
                    const auto N  = lattice.num();
                    const auto N2 = N * N;

                    assert((lattice.operators.size() == 1) && (lattice.operators.size() == 3));

                    // TODO: handle invalid terms
                    if(t.operators.size() == 1) {
                        return t.operators.front().index1;
                    }
                    return N + N2 * t.operators[0].index1 + N * t.operators[1].index1
                           + t.operators[2].index1;
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

                template <typename MomentumSpace>
                Basis3Operator(Index q_idx, const MomentumSpace& momentum_space)
                {
                    // TODO: emplace?
                    const auto N = momentum_space.num();
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

                template <typename Lattice>
                Index operator()(const Term& t, const Lattice& lattice) const
                {
                    const auto N  = lattice.num();

                    assert((lattice.operators.size() == 1) && (lattice.operators.size() == 3));

                    // TODO: handle invalid terms
                    if(t.operators.size() == 1) {
                        return t.operators.front().index1;
                    }
                    return 1 + N * t.operators[0].index1 + t.operators[1].index1;
                }
            };
        } /* namespace momentum_space */
    } /* namespace hubbard */
} /* namespace ieompp */

#endif
