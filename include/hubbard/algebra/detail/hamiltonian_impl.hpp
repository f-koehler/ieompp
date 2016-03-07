#include <algorithm>
#include "hubbard/algebra/hamiltonian.hpp"

namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        Hamiltonian<Term>::Hamiltonian()
            : J(1.), U(1.)
        {
        }

        template <typename Term>
        template <typename Discretization>
        TermList<Term> Hamiltonian<Term>::commutate(Term term,
                                                    const Discretization& discretization) const
        {
            TermList<Term> list;
            commutate(term, discretization, list);
            return list;
        }

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate(const Term term, const Discretization& discretization,
                                          TermList<Term>& result) const
        {
            commutate_hopping(term, discretization, result);
            commutate_interaction(term, discretization, result);
        }

        template <typename Term>
        template <typename Discretization>
        TermList<Term> Hamiltonian<Term>::commutate(const TermList<Term>& terms,
                                                    const Discretization& discretization)
        {
            TermList<Term> list;
            commutate(terms, discretization, list);
        }

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate(const TermList<Term>& terms,
                                          const Discretization& discretization,
                                          TermList<Term>& result)
        {
            auto num = terms.size();
            for(auto i = 0ul; i < num; ++i) commutate(terms[i], discretization, result);
        }

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate_hopping(Term term, const Discretization& discretization,
                                                  TermList<Term>& result) const
        {
            Term first = algebra::make_term(
                Prefactor(-J), {algebra::make_creator(discretization.indices[0], true),
                                algebra::make_annihilator(discretization.indices[0], true)});
            Term second = algebra::make_term(
                Prefactor(-J), {algebra::make_creator(discretization.indices[0], true),
                                algebra::make_annihilator(discretization.indices[0], true)});

            for(auto& index : discretization.indices) {
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

        template <typename Term>
        template <typename Discretization>
        void Hamiltonian<Term>::commutate_interaction(Term term,
                                                      const Discretization& discretization,
                                                      TermList<Term>& result) const
        {
            // quadrilinear term
            Term curr = algebra::make_term(
                Prefactor(U), {
                                  algebra::make_creator(discretization.indices[0], true),
                                  algebra::make_annihilator(discretization.indices[0], true),
                                  algebra::make_creator(discretization.indices[0], false),
                                  algebra::make_annihilator(discretization.indices[0], false),
                              });
            for(auto& index : discretization.indices) {
                curr.operators[0].index = index;
                curr.operators[1].index = index;
                curr.operators[2].index = index;
                curr.operators[3].index = index;

                algebra::commutate(curr, term, result);
            }

            // bilinear term 1
            curr.prefactor = -U / 2.;
            curr.operators.erase(curr.operators.begin() + 2, curr.operators.end());
            for(auto& index : discretization.indices) {
                curr.operators[0].index = index;
                curr.operators[1].index = index;

                algebra::commutate(curr, term, result);
            }

            // bilinear term 2
            curr.operators[0].spin = false;
            curr.operators[1].spin = false;
            for(auto& index : discretization.indices) {
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
