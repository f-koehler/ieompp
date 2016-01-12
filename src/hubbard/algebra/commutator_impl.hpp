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
        TermList<Term> commutate(const Term& a, const Term& b)
        {
            const auto& a_ops = a.operators;
            const auto& b_ops = b.operators;
            const auto n      = a_ops.size();
            const auto m      = b_ops.size();
            using UInt        = decltype(a_ops.size());

            assert(m * n % 2 == 0);

            TermList<Term> list;

            const auto prefactor = a.prefactor * b.prefactor;
            for(UInt k = 1; k <= n; ++k) {
                for(UInt l = 1; l <= m; ++l) {
                    if(anticommutates(a_ops[k - 1], b_ops[l - 1])) continue;

                    Real coefficient = 1.;
                    if((m * (n - k) + l - 1) % 2) coefficient = -1.;

                    Term&& new_term    = Term();
                    new_term.prefactor = prefactor;
                    new_term.prefactor *= coefficient;
                    new_term.prefactor *= anticommutator<typename Term::OperatorType, typename Term::PrefactorType>(a_ops[k - 1], b_ops[l - 1]);

                    auto& new_ops = new_term.operators;

                    if(k > 1)
                        std::copy(a_ops.begin(), a_ops.begin() + k - 1, std::back_inserter(new_ops));
                    if(l > 1)
                        std::copy(b_ops.begin(), b_ops.begin() + l - 1, std::back_inserter(new_ops));
                    std::copy(b_ops.begin() + l, b_ops.end(), std::back_inserter(new_ops));
                    std::copy(a_ops.begin() + k, a_ops.end(), std::back_inserter(new_ops));

                    list.emplace_back(new_term);
                }
            }
            return list;
        }

        template <typename Term>
        void order_term(Term& term, TermList<Term>& list)
        {
            auto unordered = find_unordered_operator(term);
            if(unordered == term.operators.end()) return;
            if(anticommutates(*unordered, *(unordered + 1))) {
                std::swap(*unordered, *(unordered + 1));
                term.prefactor *= -1.;
                order_term(term, list);
                return;
            }

            Term new_term;
            new_term.prefactor = term.prefactor;
            // TODO: add anticommutator
            /* new_term.prefactor *= */
            /*     anticommutator<Term::OperatorType, Term::PrefactorType>(unordered, unordered + 1); */
            std::copy(term.operators.begin(), unordered, std::back_inserter(new_term.operators));
            std::copy(unordered + 2, term.operators.end(), std::back_inserter(new_term.operators));
            std::swap(*unordered, *(unordered + 1));
            term.prefactor *= -1.;
            list.push_back(new_term);
            order_term(term, list);
            order_term(new_term, list);
        }
    }
}
