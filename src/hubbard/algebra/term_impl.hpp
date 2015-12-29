#include "hubbard/algebra/term.hpp"

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
            strm << term.prefactor << "\u22C5";
            for(auto& op : term.operators)
                strm << " " << op;
            return strm;
        }


        template <typename Prefactor, typename Operator>
        Term<Operator, Prefactor> make_term(const Prefactor& prefactor,
                                            const std::initializer_list<Operator>& operators)
        {
            return Term<Operator, Prefactor>{ prefactor, operators };
        }

    }
}
