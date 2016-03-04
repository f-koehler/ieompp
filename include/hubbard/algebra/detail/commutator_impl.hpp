#include "hubbard/algebra/commutator.hpp"

#include <cassert>

namespace hubbard
{
    namespace algebra
    {

        template <typename Operator>
        inline bool anticommutates(const Operator& a, const Operator& b)
        {
            return (a.creator == b.creator)
                || (a.spin    != b.spin)
                || (a.index   != b.index);
        }

        template <typename Operator, typename Prefactor>
        inline Prefactor anticommutator(const Operator&, const Operator&)
        {
            return 1;
        }

        template <typename Term>
        void commutate(const Term& a, const Term& b, TermList<Term>& terms)
        {
            const auto& a_ops = a.operators;
            const auto& b_ops = b.operators;
            const auto n      = a_ops.size();
            const auto m      = b_ops.size();
            using UInt        = decltype(a_ops.size());

            assert(m * n % 2 == 0);

            const auto prefactor = a.prefactor * b.prefactor;
            for(UInt k = 1; k <= n; ++k) {
                for(UInt l = 1; l <= m; ++l) {
                    if(anticommutates(a_ops[k - 1], b_ops[l - 1])) continue;

                    Real coefficient = 1.;
                    if((m * (n - k) + l - 1) % 2) coefficient = -1.;

                    Term&& new_term    = Term();
                    new_term.prefactor = prefactor;
                    new_term.prefactor *= coefficient;
                    new_term.prefactor *=
                        anticommutator<typename Term::Operator, typename Term::Prefactor>(
                            a_ops[k - 1], b_ops[l - 1]);

                    auto& new_ops = new_term.operators;

                    if(k > 1)
                        std::copy(a_ops.begin(), a_ops.begin() + k - 1, std::back_inserter(new_ops));
                    if(l > 1)
                        std::copy(b_ops.begin(), b_ops.begin() + l - 1, std::back_inserter(new_ops));
                    std::copy(b_ops.begin() + l, b_ops.end(), std::back_inserter(new_ops));
                    std::copy(a_ops.begin() + k, a_ops.end(), std::back_inserter(new_ops));

                    terms.emplace_back(new_term);
                }
            }
        }
    }
}
