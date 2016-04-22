#include "ieompp/algebra/commutator.hpp"

#include <cassert>
#include <iostream>

namespace ieompp
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

                    typename Term::Real coefficient = 1.;
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

        template <typename Term>
        TermList<Term> order_operators(TermList<Term>& terms)
        {
            TermList<Term> results;

            std::size_t term_idx = 0;
            while(term_idx < terms.size()) {
                auto pos = terms[term_idx].find_first_displaced_operator();
                while(pos != terms[term_idx].operators.end()) {
                    auto& term = terms[term_idx];
                    if(!anticommutates(*pos, *(pos + 1))) {
                        Term new_term;
                        new_term.prefactor = term.prefactor;
                        std::copy(term.operators.begin(), pos,
                                  std::back_inserter(new_term.operators));
                        std::copy(pos + 2, term.operators.end(),
                                  std::back_inserter(new_term.operators));
                        std::swap(*pos, *(pos + 1));
                        term.prefactor = -term.prefactor;
                        pos = terms[term_idx].find_first_displaced_operator();
                        terms.push_back(new_term);
                        continue;
                    }

                    std::swap(*pos, *(pos + 1));
                    term.prefactor = -term.prefactor;
                    pos            = terms[term_idx].find_first_displaced_operator();
                }

                auto& term = terms[term_idx];
                auto insert_pos =
                    std::find_if(results.begin(), results.end(),
                                 [&term](const Term& t) { return t.same_operators(term); });
                if(insert_pos == results.end())
                    results.push_back(term);
                else
                    insert_pos->prefactor += term.prefactor;
                ++term_idx;
            }

            return results;
        }
    }
}
