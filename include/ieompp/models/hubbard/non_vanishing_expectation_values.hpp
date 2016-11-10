#ifndef IEOMPP_MODELS_HUBBARD_NON_VANISHING_EXPECTATION_VALUES_HPP_
#define IEOMPP_MODELS_HUBBARD_NON_VANISHING_EXPECTATION_VALUES_HPP_

#include <algorithm>
#include <vector>

#include <omp.h>

#include "ieompp/models/hubbard/basis.hpp"
#include "ieompp/models/hubbard/excited_fermi_sea.hpp"

namespace ieompp
{
    namespace hubbard
    {
        namespace momentum_space
        {
            template <typename Index, typename Float>
            class NonVanishingExpectationValues : public std::vector<std::pair<Index, Index>>
            {
                template <typename Term, typename MomentumSpace, typename Dispersion>
                NonVanishingExpectationValues(const Basis3Operator<Term>& basis,
                                              const Dispersion& dispersion,
                                              const Float& fermi_energy = 0.)
                {
                    const auto basis_size = basis.size();

                    // compute conjugate basis operators
                    std::vector<Term> conjugate_basis(basis.size());
#pragma omp parallel for
                    for(decltype(basis_size) i = 0; i < basis_size; ++i) {
                        conjugate_basis[i] = basis[i].get_conjugate();
                    }

                    // apply conjugate basis operators to FermFermi sea
                    using Excitation = ExcitedFermiSea<typename MomentumSpace::Index>;
                    std::vector<Excitation> excited_states(basis_size);
#pragma omp parallel for
                    for(decltype(basis_size) i = 0; i < basis_size; ++i) {
                        excited_states[i] = Excitation(conjugate_basis, dispersion, fermi_energy);
                    }

                    // store all non-vanishing combinations in vectors
                    std::vector<std::vector<std::pair<Index, Index>>> non_vanishing(
                        omp_get_max_threads());
#pragma omp parallel for
                    for(decltype(basis_size) i = 0; i < basis_size; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(decltype(basis_size) j = 0; j < i; ++j) {
                            if(have_vanishing_overlap(excited_states[i], excited_states[j])) {
                                continue;
                            }
                            non_vanishing[thread].push_back(std::make_pair(i, j));
                            non_vanishing[thread].push_back(std::make_pair(j, i));
                        }
                    }

                    // merge vectors into one
                    std::size_t total_size = 0;
                    for(const auto& vec : non_vanishing) total_size += vec.size();
                    this->reserve(total_size);
                    for(const auto& vec : non_vanishing) {
                        this->insert(this->end(), vec.begin(), vec.end());
                    }
                }
            };
        }
    }
}

#endif
