#ifndef IEOMPP_ALGEBRA_TERM_DESCRIPTION_HPP_
#define IEOMPP_ALGEBRA_TERM_DESCRIPTION_HPP_

#include "ieompp/algebra/term/term.hpp"
#include "ieompp/description.hpp"

namespace ieompp
{
    template <typename Prefactor, typename Operator, typename Container>
    struct TypeDescription<algebra::Term<Prefactor, Operator, Container>> {
        static Description description()
        {
            return Description{{"term of fermionic operators", ""}, {"prefactor", ""}}
                   + get_type_description<Prefactor>().indent(2) + Description{{"operator", ""}}
                   + get_type_description<Operator>().indent(2);
            // TODO: add container description
        }
    };
} // namespace ieompp

#endif
