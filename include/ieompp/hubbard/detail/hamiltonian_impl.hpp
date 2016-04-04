#include <algorithm>
#include "ieompp/hubbard/hamiltonian.hpp"

namespace ieompp
{
    namespace hubbard
    {
        template <typename Term>
        Hamiltonian<Term>::Hamiltonian()
            : J(1.), U(1.), enable_hopping(true), enable_interaction(true)
        {
        }

        template <typename Term>
        template <typename Discretization>
        algebra::TermList<Term>
        Hamiltonian<Term>::commutate(Term term, const Discretization& discretization) const
        {
            algebra::TermList<Term> list;
            commutate(term, discretization, list);
            return list;
        }

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate(const Term term, const Discretization& discretization,
                                          algebra::TermList<Term>& result) const
        {
            if(enable_hopping) commutate_hopping(term, discretization, result);

            if(enable_interaction) commutate_interaction(term, discretization, result);
        }

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate_hopping(Term term, const Discretization& discretization,
                                                  algebra::TermList<Term>& result) const
        {
            Term first;
            first.prefactor = -J;
            first.operators.resize(2);
            first.operators[0].creator = true;
            first.operators[1].creator = false;
            Term second;
            second.prefactor = -J;
            second.operators.resize(2);
            second.operators[0].creator = true;
            second.operators[1].creator = false;

            for(int spin = 0; spin <= 1; ++spin) {
                first.operators[0].spin  = spin;
                first.operators[1].spin  = spin;
                second.operators[0].spin = spin;
                second.operators[1].spin = spin;
                for(auto& index : discretization) {
                    auto neighbours           = discretization.unique_neighbours(index);
                    first.operators[0].index  = index;
                    second.operators[1].index = index;
                    for(auto& neigh : neighbours) {
                        first.operators[1].index  = neigh;
                        second.operators[0].index = neigh;
                        algebra::commutate(first, term, result);
                        algebra::commutate(second, term, result);
                    }
                }
            }
        }

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate_interaction(Term term,
                                                      const Discretization& discretization,
                                                      algebra::TermList<Term>& result) const
        {
            Term curr;
            curr.prefactor = U;
            curr.operators.resize(4);
            curr.operators[0].spin    = true;
            curr.operators[1].spin    = true;
            curr.operators[2].spin    = false;
            curr.operators[3].spin    = false;
            curr.operators[0].creator = true;
            curr.operators[1].creator = false;
            curr.operators[2].creator = true;
            curr.operators[3].creator = false;
            for(auto& index : discretization) {
                curr.operators[0].index = index;
                curr.operators[1].index = index;
                curr.operators[2].index = index;
                curr.operators[3].index = index;

                algebra::commutate(curr, term, result);
            }

            // bilinear term 1
            curr.prefactor = -U / 2.;
            curr.operators.erase(curr.operators.begin() + 2, curr.operators.end());
            for(auto& index : discretization) {
                curr.operators[0].index = index;
                curr.operators[1].index = index;

                algebra::commutate(curr, term, result);
            }

            // bilinear term 2
            curr.operators[0].spin = false;
            curr.operators[1].spin = false;
            for(auto& index : discretization) {
                curr.operators[0].index = index;
                curr.operators[1].index = index;

                algebra::commutate(curr, term, result);
            }
        }

        template <typename Term>
        template <typename RealSpace>
        typename RealSpace::Real Hamiltonian<Term>::dispersion(const RealSpace& discretization,
                                            const typename RealSpace::Vector& momentum)
        {
            typename RealSpace::Real energy;
            for(auto& vector : discretization.lattice_vectors) {
                energy += std::cos(RealSpace::dot_product(vector, momentum));
            }
            return -2. * J * energy;
        }
    }
}
