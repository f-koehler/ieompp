#ifndef IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_NON_VANISHING_EXPECTATION_VALUES_HPP_
#define IEOMPP_MODELS_HUBBARD_MOMENTUM_SPACE_NON_VANISHING_EXPECTATION_VALUES_HPP_

#include "ieompp/models/hubbard_momentum_space/basis.hpp"
#include "ieompp/models/hubbard_momentum_space/excited_fermi_sea.hpp"
#include "ieompp/openmp.hpp"

#include <algorithm>
#include <vector>

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
                template <typename Monomial, typename Dispersion>
                NonVanishingExpectationValues(const Basis3Operator<Monomial>& basis,
                                              const Dispersion& dispersion,
                                              const Float& fermi_energy = 0.)
                {
                    const auto basis_size = basis.size();

                    // compute conjugate basis operators
                    std::vector<Monomial> conjugate_basis(basis.size());
#pragma omp parallel for
                    for(typename Basis3Operator<Monomial>::Index i = 0; i < basis_size; ++i) {
                        conjugate_basis[i] = basis[i].get_conjugate();
                    }

                    // apply conjugate basis operators to FermFermi sea
                    using Excitation = ExcitedFermiSea<Monomial>;
                    std::vector<Excitation> excited_states(basis_size);
#pragma omp parallel for
                    for(typename Basis3Operator<Monomial>::Index i = 0; i < basis_size; ++i) {
                        excited_states[i] =
                            Excitation(conjugate_basis[i], dispersion, fermi_energy);
                    }

                    // store all non-vanishing combinations in vectors
                    std::vector<std::vector<std::pair<Index, Index>>> non_vanishing(
                        omp_get_max_threads());
#pragma omp parallel for schedule(dynamic, 1)
                    for(typename Basis3Operator<Monomial>::Index i = 0; i < basis_size; ++i) {
                        const auto thread = omp_get_thread_num();
                        if(excited_states[i].vanishes) continue;
                        non_vanishing[thread].push_back(std::make_pair(i, i));
                        for(typename Basis3Operator<Monomial>::Index j = 0; j < i; ++j) {
                            if(excited_states[j].vanishes) continue;
                            if(excited_states[i] != excited_states[j]) {
                                continue;
                            }
                            non_vanishing[thread].push_back(std::make_pair(i, j));
                        }
                    }

                    // merge vectors into one
                    std::size_t total_size = 0;
                    for(const auto& vec : non_vanishing) {
                        total_size += vec.size();
                    }
                    this->reserve(total_size);
                    for(const auto& vec : non_vanishing) {
                        this->insert(this->end(), vec.begin(), vec.end());
                    }
                }
            };
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
