#ifndef HUBBARD_ALGEBRA_TERM_HPP_
#define HUBBARD_ALGEBRA_TERM_HPP_

#include "hubbard/types.hpp"

#include <vector>

template<typename Operator, typename Prefactor = Complex>
struct Term {
    Prefactor prefactor;
    std::vector<Operator> operators;

    inline bool same_operators(const Term& rhs) const;
    inline bool ordered() const;

    inline bool operator==(const Term& rhs) const;
    inline bool operator!=(const Term& rhs) const;
};

template<typename Term>
struct TermList : public std::vector<Term> {
};

template<typename Operator, typename Prefactor>
std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term);

#include "hubbard/algebra/term_impl.hpp"

#endif
