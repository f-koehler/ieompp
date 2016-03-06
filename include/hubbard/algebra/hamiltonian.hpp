#ifndef HUBBARD_ALGEBRA_HAMILTONIAN_HPP_
#define HUBBARD_ALGEBRA_HAMILTONIAN_HPP_

#include "hubbard/algebra/commutator.hpp"
#include "hubbard/algebra/operator.hpp"
#include "hubbard/algebra/term.hpp"

namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        struct Hamiltonian {
            using Prefactor = typename Term::Prefactor;

            Prefactor J, U;

            Hamiltonian();

            template <typename Discretization>
            TermList<Term> commutate(const Term& term, const Discretization& discretization) const;

            template <typename Discretization>
            void commutate(const Term& term, const Discretization& discretization,
                           TermList<Term>& result) const;

            template <typename Discretization>
            void commutate(const TermList<Term>& terms, const Discretization& discretization,
                           const TermList<Term>& result);

            template <typename Discretization>
            void commutate_hopping(const Term& term, const Discretization& discretization,
                                   TermList<Term>& result) const;

            template <typename Discretization>
            void commutate_interaction(const Term& term, const Discretization& discretization,
                                       TermList<Term>& result) const;
        };
    }
}

#include "detail/hamiltonian_impl.hpp"

#endif
