#include "hubbard/algebra/term.hpp"

#include <algorithm>

namespace hubbard
{
    namespace algebra {

        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::same_operators(const Term<Operator, Prefactor>& rhs) const
        {
            if(operators.size() != rhs.operators.size())
                return false;
            return std::equal(operators.begin(), operators.end(), rhs.operators.begin());
        }


        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::operator==(const Term<Operator, Prefactor>& rhs) const
        {
            return same_operators(rhs) && prefactor == rhs.prefactor;
        }

        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::operator!=(const Term<Operator, Prefactor>& rhs) const
        {
            return !same_operators(rhs) || prefactor != rhs.prefactor;
        }

        template <typename Operator, typename Prefactor>
        std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term)
        {
            strm << term.prefactor;
            if(!term.operators.empty()) {
                strm << u8"⋅";
                for(auto& op : term.operators) strm << " " << op;
            }
            return strm;
        }

        template <typename Prefactor, typename Operator>
        Term<Operator, Prefactor> make_term(const Prefactor& prefactor,
                                            const std::initializer_list<Operator>& operators)
        {
            return Term<Operator, Prefactor>{prefactor, operators};
        }

        template <typename Term>
        void sum_terms(TermList<Term>& terms)
        {
            TermList<Term> reduced;
            for(auto& term : terms) {
                auto pos = std::find_if(reduced.begin(), reduced.end(),
                                        [&term](const Term& t) { return term.same_operators(t); });
                if(pos == reduced.end()) {
                    reduced.push_back(term);
                    continue;
                }
                pos->prefactor += term.prefactor;
            }
            std::swap(reduced, terms);
        }
    }
}
