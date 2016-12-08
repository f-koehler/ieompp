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
                    // -> calculate b_i^† |FS>
                    for(BasisIndex i = 0; i < basis_size; ++i) {
                        states[i].apply_monomial(conjugate_basis[i], dispersion, fermi_energy);
                    }

                    // apply basis monomials and check for non-vanishing combinations
                    for(BasisIndex i = 0; i < basis_size; ++i) {
                        // skip state if it already vanishes
                        if(states[i].vanishes) {
                            continue;
                        }

                        for(BasisIndex j = 0; j < basis_size; ++j) {
                            // get a working copy of state b_i^† |FS>
                            State state = states[i];

                            // calculate b_j b_i^† |FS>
                            state.apply_monomial(basis[j], dispersion, fermi_energy);

                            // check if state is fermi sea
                            if(state.is_initial_fermi_sea()) {
                                // <FS| b_j b_i^† |FS> = 1
                                // store index combination j, i
                                this->emplace_back(std::pair<Index, Index>{j, i});
                            }
                        }
                    }
                }

                void sort()
                {
                    std::sort(this->begin(), this->end(), [](const std::pair<Index, Index>& a,
                                                             const std::pair<Index, Index>& b) {
                        if(a.first < b.first) return true;
                        if(a.first == b.first) return (a.second < b.second);
                        return false;
                    });
                }
            };
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
