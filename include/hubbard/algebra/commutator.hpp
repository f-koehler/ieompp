#ifndef COMMUTATOR_HPP_
#define COMMUTATOR_HPP_

#include "hubbard/algebra/term.hpp"

template<typename Operator>
inline bool anticommutates(const Operator& a, const Operator& b);

template<typename Operator, typename Prefactor = Complex>
inline Prefactor anticommutator(const Operator& a, const Operator& b);

template<typename Term>
TermList<Term> commutate(const Term& a, const Term& b);

#include "hubbard/algebra/commutator_impl.hpp"

#endif
