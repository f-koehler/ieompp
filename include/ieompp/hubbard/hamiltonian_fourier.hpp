#ifndef IEOMPP_HUBBARD_HAMILTONIAN_FOURIER_HPP_
#define IEOMPP_HUBBARD_HAMILTONIAN_FOURIER_HPP_

#include "ieompp/algebra/commutator.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"

namespace ieompp
{
    namespace hubbard
    {
        template <typename TermT>
        struct HamiltonianFourier {
            using Term      = TermT;
            using Prefactor = typename Term::Prefactor;

            typename Prefactor::value_type J, U;
            bool enable_hopping, enable_interaction;

            HamiltonianFourier();

            template <typename FourierSpace, typename RealSpace>
            algebra::TermList<Term> commutate(Term term, const FourierSpace& discretization,
                                              const RealSpace& lattice) const;

            template <typename FourierSpace, typename RealSpace>
            void commutate(Term term, const FourierSpace& discretization, const RealSpace& lattice,
                           algebra::TermList<Term>& result) const;

            template <typename FourierSpace, typename RealSpace>
            void commutate_hopping(Term term, const FourierSpace& discretization,
                                   const RealSpace& lattice, algebra::TermList<Term>& result) const;

            template <typename FourierSpace, typename RealSpace>
            void commutate_interaction(Term term, const FourierSpace& discretization,
                                       const RealSpace& lattice,
                                       algebra::TermList<Term>& result) const;
        };
    }
}

#include "detail/hamiltonian_fourier_impl.hpp"

#endif
