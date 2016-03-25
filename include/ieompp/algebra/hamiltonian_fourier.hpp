#ifndef IEOMPP_ALGEBRA_HAMILTONIAN_FOURIER_HPP_
#define IEOMPP_ALGEBRA_HAMILTONIAN_FOURIER_HPP_

#include "ieompp/algebra/commutator.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <typename Term>
        struct HamiltonianFourier {
            using Prefactor = typename Term::Prefactor;

            typename Prefactor::value_type J, U;
            bool enable_hopping, enable_interaction;

            HamiltonianFourier();

            template <typename FourierSpace, typename RealSpace>
            TermList<Term> commutate(Term term, const RealSpace& lattice,
                                     const FourierSpace& discretization) const;

            template <typename FourierSpace, typename RealSpace>
            void commutate(Term term, const FourierSpace& discretization, const RealSpace& lattice,
                           TermList<Term>& result) const;

            template <typename FourierSpace, typename RealSpace>
            void commutate_hopping(Term term, const FourierSpace& discretization,
                                   const RealSpace& lattice, TermList<Term>& result) const;

            template <typename FourierSpace, typename RealSpace>
            void commutate_interaction(Term term, const FourierSpace& discretization,
                                       const RealSpace& lattice, TermList<Term>& result) const;
        };
    }
}

#include "detail/hamiltonian_fourier_impl.hpp"

#endif
