#include "hubbard/algebra/hamiltonian_fourier.hpp"

#include <cmath>

namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        HamiltonianFourier<Term>::HamiltonianFourier()
            : J(1.), U(1.)
        {
        }

        template <typename Term>
        template <typename FourierSpace, typename RealSpace>
        TermList<Term> HamiltonianFourier<Term>::commutate(Term term, const RealSpace& lattice,
                                                           const FourierSpace& discretization) const
        {
            TermList<Term> list;
            commutate(term, discretization, lattice, list);
            return list;
        }

        template <typename Term>
        template <typename FourierSpace, typename RealSpace>
        void HamiltonianFourier<Term>::commutate(Term term, const FourierSpace& discretization,
                                                 const RealSpace& lattice,
                                                 TermList<Term>& result) const
        {
            commutate_hopping(term, discretization, lattice, result);
            commutate_interaction(term, discretization, lattice, result);
        }

        template <typename Term>
        template <typename FourierSpace, typename RealSpace>
        void HamiltonianFourier<Term>::commutate_hopping(Term term,
                                                         const FourierSpace& discretization,
                                                         const RealSpace& lattice,
                                                         TermList<Term>& result) const
        {
            Term t;
            t.operators.resize(2);
            t.operators[0].creator = true;
            t.operators[1].creator = false;

            for(auto& idx : discretization.indices) {
                auto& k = discretization[idx];
                t.prefactor = 0.;
                t.operators[0].index = idx;
                t.operators[1].index = idx;
                for(auto& delta : lattice.lattice_vectors) {
                    t.prefactor += std::cos(dot_product(k, delta));
                }
                t.prefactor *= -2. * J;
                // TODO: we need a proper spin class with all spin values
                for(auto spin = 0; spin <= 1; ++spin) {
                    t.operators[0].spin = spin;
                    t.operators[1].spin = spin;
                    algebra::commutate(t, term, result);
                }
            }
        }

        template <typename Term>
        template <typename FourierSpace, typename RealSpace>
        void HamiltonianFourier<Term>::commutate_interaction(Term term,
                                                             const FourierSpace& discretization,
                                                             const RealSpace& lattice,
                                                             TermList<Term>& result) const
        {
            Term t;
            t.prefactor = U / lattice.indices.size();
            t.operators.resize(4);
            t.operators[0].creator = true;
            t.operators[1].creator = false;
            t.operators[2].creator = true;
            t.operators[3].creator = false;
            t.operators[0].spin = true;
            t.operators[1].spin = true;
            t.operators[2].spin = false;
            t.operators[3].spin = false;

            // quadrilinear term
            for(auto& idx1 : discretization.indices) {
                auto& k1 = discretization[idx1];
                t.operators[0].index = idx1;
                for(auto& idx2 : discretization.indices) {
                    auto& k2 = discretization[idx2];
                    t.operators[1].index = idx2;
                    for(auto& idx3 : discretization.indices) {
                        auto& k3 = discretization[idx3];
                        t.operators[2].index = idx3;

                        auto k4 = k1 + k3 - k2;
                        if(discretization.out_of_bounds(k4)) continue;
                        auto idx4 = discretization[k4];
                        t.operators[3].index = idx4;

                        algebra::commutate(t, term, result);
                    }
                }
            }

            // bilinear terms
            t.prefactor = -0.5 * U / lattice.indices.size();
            t.operators.resize(2);
            for(auto& idx : discretization.indices) {
                auto& k = discretization[idx];
                t.operators[0].index = idx;
                t.operators[1].index = idx;
                t.operators[0].spin  = true;
                t.operators[1].spin  = true;
                algebra::commutate(t, term, result);

                t.operators[0].spin = false;
                t.operators[1].spin = false;
                algebra::commutate(t, term, result);
            }
        }
    }
}
