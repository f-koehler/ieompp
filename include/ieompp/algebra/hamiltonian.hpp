#ifndef IEOMPP_ALGEBRA_HAMILTONIAN_HPP_
#define IEOMPP_ALGEBRA_HAMILTONIAN_HPP_

#include "ieompp/algebra/commutator.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <typename Term>
        struct Hamiltonian {
            using Prefactor = typename Term::Prefactor;

            typename Prefactor::value_type J, U;
            bool enable_hopping, enable_interaction;

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
