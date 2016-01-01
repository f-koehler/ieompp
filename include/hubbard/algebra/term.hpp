#ifndef HUBBARD_ALGEBRA_TERM_HPP_
#define HUBBARD_ALGEBRA_TERM_HPP_

#include "hubbard/types.hpp"

#include <vector>

namespace hubbard
{
    namespace algebra
    {
        template <typename Operator, typename Prefactor = Complex>
        struct Term {
            using OperatorType  = Operator;
            using PrefactorType = Prefactor;

            Prefactor prefactor;
            std::vector<Operator> operators;

            inline bool same_operators(const Term& rhs) const;
            inline bool ordered() const;

            inline bool operator==(const Term& rhs) const;
            inline bool operator!=(const Term& rhs) const;
        };

        template <typename Term>
        class TermList : public std::vector<Term>
        {
        };

        template <typename Operator, typename Prefactor>
        std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term);


        template <typename Prefactor, typename Operator>
        Term<Operator, Prefactor> make_term(const Prefactor& prefactor,
                                            const std::initializer_list<Operator>& operators);

        template <typename Term>
        auto find_unordered_operator(Term& term) ->
            typename std::vector<typename Term::OperatorType>::iterator;
    }
}


#include "hubbard/algebra/term_impl.hpp"

#endif
