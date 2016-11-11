#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_NON_VANISHING_EXPECTATION_VALUES_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_NON_VANISHING_EXPECTATION_VALUES_HPP_

#include <algorithm>
#include <vector>

#include <omp.h>

#include "ieompp/models/hubbard_momentum_space/basis.hpp"
#include "ieompp/models/hubbard_momentum_space/excited_fermi_sea.hpp"

namespace ieompp
{
    namespace models
    {
        namespace hubbard_momentum_space
        {
            template <typename Index, typename Float>
            class NonVanishingExpectationValues : public std::vector<std::pair<Index, Index>>
            {
            public:
                template <typename Term, typename Dispersion>
                NonVanishingExpectationValues(const Basis3Operator<Term>& basis,
                                              const Dispersion& dispersion,
                                              const Float& fermi_energy = 0.)
                {
                    const auto basis_size = basis.size();

                    // compute conjugate basis operators
                    std::vector<Term> conjugate_basis(basis.size());
#pragma omp parallel for
                    for(typename Basis3Operator<Term>::Index i = 0; i < basis_size; ++i) {
                        conjugate_basis[i] = basis[i].get_conjugate();
                    }

                    // apply conjugate basis operators to FermFermi sea
                    using Excitation = ExcitedFermiSea<typename Term::Operator::Index1>;
                    std::vector<Excitation> excited_states(basis_size);
#pragma omp parallel for
                    for(typename Basis3Operator<Term>::Index i = 0; i < basis_size; ++i) {
                        excited_states[i] =
                            Excitation(conjugate_basis[i], dispersion, fermi_energy);
                    }

                    /* // store all non-vanishing combinations in vectors */
                    /* std::vector<std::vector<std::pair<Index, Index>>> non_vanishing( */
                    /*     omp_get_max_threads()); */
                    /* #pragma omp parallel for */
                    /* for(typename Basis3Operator<Term>::Index i = 0; i < basis_size; ++i) { */
                    /*     const auto thread = omp_get_thread_num(); */
                    /*     for(typename Basis3Operator<Term>::Index j = 0; j < i; ++j) { */
                    /*         if(have_vanishing_overlap(excited_states[i], excited_states[j])) { */
                    /*             continue; */
                    /*         } */
                    /*         non_vanishing[thread].push_back(std::make_pair(i, j)); */
                    /*         non_vanishing[thread].push_back(std::make_pair(j, i)); */
                    /*     } */
                    /* } */

                    /* // merge vectors into one */
                    /* std::size_t total_size = 0; */
                    /* for(const auto& vec : non_vanishing) { */
                    /*     total_size += vec.size(); */
                    /* } */
                    /* this->reserve(total_size); */
                    /* for(const auto& vec : non_vanishing) { */
                    /*     this->insert(this->end(), vec.begin(), vec.end()); */
                    /* } */
                }
            };
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
