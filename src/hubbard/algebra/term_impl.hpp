#include "hubbard/algebra/term.hpp"
#include "hubbard/algebra/commutator.hpp"

namespace hubbard
{
    namespace algebra {
        
        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::ordered() const
        {
            bool annihilator = false;
            for(auto& op : operators) {
                if(op.creator) {
                    if(annihilator) return false;
                } else {
                    annihilator = true;
                }
            }
            return true;
        }

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
        auto find_unordered_operator(Term& term) ->
            typename std::vector<typename Term::OperatorType>::iterator
        {
            bool annihilator = false;
            for(auto iter = term.operators.begin(); iter != term.operators.end(); ++iter) {
                if(!iter->creator)
                    annihilator = true;
                else if(annihilator)
                    return iter-1;
            }
            return term.operators.end();
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
