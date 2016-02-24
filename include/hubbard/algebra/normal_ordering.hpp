#ifndef HUBBARD_ALGEBRA_NORMAL_ORDERING_HPP_
#define HUBBARD_ALGEBRA_NORMAL_ORDERING_HPP_

#include <functional>

#include "hubbard/algebra/term.hpp"

namespace hubbard
{
    namespace algebra
    {
        template<typename Term>
        void normal_order(const Term& t, TermList<Term>& terms, std::function<typename Term::PrefactorType(typename Term::OperatorType::ContractionType)>);
    }
}

#endif
