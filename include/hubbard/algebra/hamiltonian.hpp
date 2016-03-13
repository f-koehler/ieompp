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

            typename Prefactor::value_type J, U;

            Hamiltonian();

            template <typename Discretization>
            TermList<Term> commutate(Term term, const Discretization& discretization) const;

            template <typename Discretization>
            void commutate(Term term, const Discretization& discretization,
                           TermList<Term>& result) const;

            template <typename Discretization>
            void commutate_hopping(Term term, const Discretization& discretization,
                                   TermList<Term>& result) const;

            template <typename Discretization>
            void commutate_interaction(Term term, const Discretization& discretization,
                                       TermList<Term>& result) const;

            template <typename RealSpace>
            typename RealSpace::Real dispersion(const RealSpace& discretization,
                                                const typename RealSpace::Vector& momentum);
        };
    }
}

#include "detail/hamiltonian_impl.hpp"

#endif
