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
            template <typename IndexT, typename FloatT>
            struct NonVanishingExpectationValue {
                using Index = IndexT;
                using Float = FloatT;

                Index left_index, right_index;
                Float value;
            };

            template <typename IndexT, typename FloatT>
            class NonVanishingExpectationValues
                : public std::vector<NonVanishingExpectationValue<IndexT, FloatT>>
            {
            public:
                using Contribution = NonVanishingExpectationValue<IndexT, FloatT>;

                template <typename Monomial, typename Dispersion>
                NonVanishingExpectationValues(const Basis3Operator<Monomial>& basis,
                                              const Basis3Operator<Monomial>& conjugate_basis,
                                              const Dispersion& dispersion,
                                              const typename Contribution::Float& fermi_energy = 0.)
                {
                    const auto basis_size = basis.size();
                    assert(basis.size() == conjugate_basis.size());

                    using State      = ExcitedFermiSea<Monomial>;
                    using BasisIndex = typename Basis3Operator<Monomial>::BasisIndex;

                    // calculate b_i^† |FS>
                    std::vector<State> states(basis_size);
                    for(BasisIndex i = 0; i < basis_size; ++i) {
                        states[i].apply_monomial(conjugate_basis[i], dispersion, fermi_energy);
                    }

                    // check if b_0 b_0^† |FS> vanishes
                    auto state = states[0];
                    state.apply_monomial(basis[0], dispersion, fermi_energy);
                    if(state.is_initial_fermi_sea()) {
                        this->emplace_back(Contribution{0, 0, 1.0});
                    }

                    // check if b_i b_0^† |FS> vanishes
                    for(BasisIndex i = 1; i < basis_size; ++i) {
                        bool vanishes                      = true;
                        typename Contribution::Float value = 0.0;

                        state = states[0];
                        state.apply_monomial(basis[i], dispersion, fermi_energy);
                        if(state.is_initial_fermi_sea()) {
                            value    = 2.0;
                            vanishes = false;
                        }

                        const bool kronecker = (basis[i][1].index1 == basis[i][2].index1);
                        if(kronecker) {
                            state = State();
                            state.apply_operator(conjugate_basis[0][0], dispersion, fermi_energy);
                            state.apply_operator(basis[i][0], dispersion, fermi_energy);
                            if(state.is_initial_fermi_sea()) {
                                value += 1.;
                                vanishes = false;
                            }
                        }

                        if(!vanishes) {
                            this->emplace_back(Contribution{i, 0, value});
                        }
                    }

                    // check if b_0 b_i^† |FS> vanishes
                    for(BasisIndex i = 1; i < basis_size; ++i) {
                        bool vanishes                      = true;
                        typename Contribution::Float value = 0.0;

                        state = states[i];
                        state.apply_monomial(basis[0], dispersion, fermi_energy);
                        if(state.is_initial_fermi_sea()) {
                            value    = 2.0;
                            vanishes = false;
                        }

                        const bool kronecker =
                            (conjugate_basis[i][0].index1 == conjugate_basis[i][1].index1);
                        if(kronecker) {
                            state = State();
                            state.apply_operator(conjugate_basis[i][2], dispersion, fermi_energy);
                            state.apply_operator(basis[0][0], dispersion, fermi_energy);
                            if(state.is_initial_fermi_sea()) {
                                value += 1.0;
                                vanishes = false;
                            }
                        }

                        if(!vanishes) {
                            this->emplace_back(Contribution{0, i, value});
                        }
                    }

                    // check if b_i b_j^† |FS> vanishes
                    for(BasisIndex i = 1; i < basis_size; ++i) {
                        for(BasisIndex j = 1; j < basis_size; ++j) {
                            bool vanishes                      = true;
                            typename Contribution::Float value = 0.;

                            state = states[j];
                            state.apply_monomial(basis[i], dispersion, fermi_energy);
                            if(state.is_initial_fermi_sea()) {
                                value    = 4.0;
                                vanishes = false;
                            }

                            const bool kronecker1 = (basis[i][1].index1 == basis[i][2].index1);
                            const bool kronecker2 =
                                (conjugate_basis[j][0].index1 == conjugate_basis[j][1].index1);

                            if(kronecker1) {
                                state = states[j];
                                state.apply_operator(basis[0][0], dispersion, fermi_energy);
                                if(state.is_initial_fermi_sea()) {
                                    value += 2.0;
                                    vanishes = false;
                                }
                            }

                            if(kronecker2) {
                                state = states[0];
                                state.apply_monomial(basis[i], dispersion, fermi_energy);
                                if(state.is_initial_fermi_sea()) {
                                    value += 2.0;
                                    vanishes = false;
                                }
                            }

                            if(kronecker1 && kronecker2) {
                                state = State();
                                state.apply_operator(conjugate_basis[j][2], dispersion,
                                                     fermi_energy);
                                state.apply_operator(basis[i][0], dispersion, fermi_energy);
                                if(state.is_initial_fermi_sea()) {
                                    value += 1.0;
                                    vanishes = false;
                                }
                            }

                            if(!vanishes) {
                                this->emplace_back(Contribution{i, j, value});
                            }
                        }
                    }
                }

                void sort()
                {
                    std::sort(this->begin(), this->end(),
                              [](const Contribution& a, const Contribution& b) {
                                  if(a.left_index < b.left_index) {
                                      return true;
                                  }
                                  if(a.left_index == b.left_index) {
                                      return (a.right_index < b.right_index);
                                  }
                                  return false;
                              });
                }
            };
        } // namespace hubbard_momentum_space
    }     // namespace models
} // namespace ieompp

#endif
