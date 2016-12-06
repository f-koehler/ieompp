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
            template <typename IndexT>
            class NonVanishingExpectationValues : public std::vector<std::pair<IndexT, IndexT>>
            {
            public:
                using Index = IndexT;

                template <typename Monomial, typename Dispersion>
                NonVanishingExpectationValues(const Basis3Operator<Monomial>& basis,
                                              const Basis3Operator<Monomial>& conjugate_basis,
                                              const Dispersion& dispersion,
                                              const typename Dispersion::Float& fermi_energy = 0.)
                {
                    const auto basis_size = basis.size();
                    assert(basis.size() == conjugate_basis.size());

                    using State      = ExcitedFermiSea<Monomial>;
                    using BasisIndex = typename Basis3Operator<Monomial>::BasisIndex;

                    std::vector<State> states(basis_size);

// apply conjugate basis monomials to fermi_sea
#pragma omp parallel for
                    for(BasisIndex i = 0; i < basis_size; ++i) {
                        states[i].apply_monomial(conjugate_basis[i], dispersion, fermi_energy);
                    }

                    // apply basis monomials and check for non-vanishing combinations
                    std::vector<std::vector<std::pair<Index, Index>>> non_vanishing(
                        omp_get_max_threads());
#pragma omp parallel for
                    for(BasisIndex i = 0; i < basis_size; ++i) {
                        if(states[i].vanishes) continue;
                        const auto thread = omp_get_thread_num();

                        for(BasisIndex j = 0; j < basis_size; ++j) {
                            State state = states[i];
                            state.apply_monomial(basis[j], dispersion, fermi_energy);
                            if(state.is_initial_fermi_sea()) {
                                non_vanishing[thread].emplace_back(std::pair<Index, Index>{j, i});
                            }
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
