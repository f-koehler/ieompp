#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_SITE_OCCUPATION_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_SITE_OCCUPATION_HPP_

#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/openmp.hpp"
#include "ieompp/types/matrix.hpp"
#include "ieompp/types/number.hpp"

#include <functional>
#include <numeric>
#include <vector>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Float, typename Basis>
            struct SiteOccupation {
            };

            template <typename FloatT, typename MonomialT>
            struct SiteOccupation<FloatT, Basis1Operator<MonomialT>> {
                using Float    = FloatT;
                using Monomial = MonomialT;
                using Basis    = Basis1Operator<Monomial>;
                using Operator = typename Monomial::Operator;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;

                ExpectationValueFunction expectation_value;
                std::reference_wrapper<const Basis1Operator<Monomial>> basis_ref;
                std::reference_wrapper<const Basis1Operator<Monomial>> conjugate_basis_ref;

                Float expectation_value_1_1(const Monomial& a, const Monomial& b) const
                {
                    assert(a.size() == 1);
                    assert(b.size() == 1);

                    return 2. * expectation_value(a.front(), b.front());
                }

                template <typename Vector>
                Float operator()(const Vector& vector) const
                {
                    const auto& basis           = basis_ref.get();
                    const auto& conjugate_basis = conjugate_basis_ref.get();
                    const auto size             = basis.size();

                    std::vector<std::complex<Float>> results(omp_get_max_threads(), 0);

#pragma omp parallel for schedule(dynamic, 1)
                    for(auto i = 0ul; i < size; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = 0ul; j < size; ++j) {
                            results[thread] +=
                                expectation_value_1_1(basis[i], basis[j])
                                * types::multiply_with_conjugate(vector[i], vector[j]) / 2.;
                        }
                    }

                    const auto result =
                        std::accumulate(results.begin(), results.end(), std::complex<Float>(0.));

                    assert(result.imag() < 1e-15);
                    return result.real();
                }
            };

            template <typename FloatT, typename MonomialT>
            struct SiteOccupation<FloatT, Basis3Operator<MonomialT>> {
                using Float    = FloatT;
                using Monomial = MonomialT;
                using Basis    = Basis3Operator<Monomial>;
                using Operator = typename Monomial::Operator;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;

                ExpectationValueFunction expectation_value;
                std::reference_wrapper<const Basis3Operator<Monomial>> basis_ref;
                std::reference_wrapper<const Basis3Operator<Monomial>> conjugate_basis_ref;

                Float expectation_value_1_1(const Monomial& a, const Monomial& b) const
                {
                    assert(a.size() == 1);
                    assert(b.size() == 1);

                    // return <N[c_{a0,↑}^†]N_[c_{b0,↑}]> = 2 * <c_{a0,↑}^† c_{b0,↑}>
                    return 2. * expectation_value(a.front(), b.front());
                }

                Float expectation_value_1_3(const Monomial& a, const Monomial& b) const
                {
                    assert(a.size() == 1);
                    assert(b.size() == 3);

                    // start with <c_{b2,↓}^† c_{b1,↓}>
                    Float ret = expectation_value(b[2], b[1]);

                    // add -0.5 δ_{b1,b2}
                    if(b[1].index1 == b[2].index1) {
                        ret -= 0.5;
                    }

                    // multiply with <c_{a0,↑}^† c_{b0,↑}>
                    ret *= expectation_value(a.front(), b.front());

                    // return <N[c_{a0,↑}^†] N[c_{b0,↑} c_{b1,↓}^† c_{b2,↓}]>=
                    // 4 * <c_{a0,↑}^† c_{b0,↑}> * (<c_{b1,↓}^† c_{b2,↓}> - 0.5 δ_{b1,b2})
                    return 4 * ret;
                }

                Float expectation_value_3_3(const Monomial& a, const Monomial& b) const
                {
                    assert(a.size() == 3);
                    assert(b.size() == 3);

                    // calculate 8 * <c_{a0,↑}^† c_{a1,↓}^† c_{a2,↓} c_{b0,↑} c_{b1,↓}^† c_{b2,↓}>
                    // = 8 * <c_{a0,↑}^† c_{b0,↑}> <c_{a1,↓}^† c_{a2,↓}> <c_{b1,↓}^† c_{b2,↓}>
                    // + 8 * <c_{a0,↑}^† c_{b0,↑}> <c_{a1,↓}^† c_{b1,↓}> (δ_{a2,b2}-<c_{b2,↓}^†
                    // c_{a2,↓}>)
                    Float ev_6 = 8 * expectation_value(a[0], b[0]) * expectation_value(a[1], a[2])
                                 * expectation_value(b[1], b[2]);
                    ev_6 += 8 * expectation_value(a[0], b[0]) * expectation_value(a[1], b[1])
                            * (((a[2].index1 == b[2].index1) ? 1. : 0.)
                               - expectation_value(b[2], a[2]));

                    Float ev_4 = 0.;
                    // calculate -4 * δ_{b1,b2} * <c_{a0,↑}^† c_{b0,↑} c_{a1,↓}^† c_{a2,↓}>
                    // = -4 * <c_{a0,↑}^† c_{b0,↑}> <c_{a1,↓}^† c_{a2,↓}>
                    if(b[1].index1 == b[2].index1) {
                        ev_4 -= 4 * expectation_value(a[0], b[0]) * expectation_value(a[1], a[2]);
                    }
                    // calculate -4 * δ_{a1,b2} * <c_{a0,↑}^† c_{b0,↑} c_{b1,↓}^† c_{b2,↓}>
                    // = -4 * <c_{a0,↑}^† c_{b0,↑}> <c_{a1,↓}^† c_{a2,↓}>
                    if(a[1].index1 == a[2].index1) {
                        ev_4 -= 4 * expectation_value(a[0], b[0]) * expectation_value(b[1], b[2]);
                    }

                    Float ev_2 = 0.;
                    // calculate 2 * δ_{a1,a2} * δ_{b1,b2} * <c_{a0,↑}^† c_{b0,↑}>
                    if((a[1].index1 == a[2].index1) && (b[1].index1 == b[2].index1)) {
                        ev_2 += 2 * expectation_value(a[0], b[0]);
                    }

                    // return <N[c_{a0,↑}^† c_{a1,↓}^† c_{a2,↓}] N[c_{b0,↑} c_{b1,↓}^† c_{b2,↓}]>
                    // by summing the individual contributions ev_2, ev_4, ev_6
                    return ev_6 + ev_4 + ev_2;
                }

                template <typename Vector>
                Float operator()(const Vector& vector) const
                {
                    const auto& basis           = basis_ref.get();
                    const auto& conjugate_basis = conjugate_basis_ref.get();
                    const auto N                = basis.N;
                    const auto basis_size       = basis.size();
                    std::vector<std::complex<Float>> results(omp_get_max_threads(), 0);

#pragma omp parallel
                    for(auto i = 0ul; i < N; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = 0ul; j < N; ++j) {
                            results[thread] +=
                                expectation_value_1_1(basis[i], basis[j]) / 2.
                                * types::multiply_with_conjugate(vector[i], vector[j]);
                        }
                    }

#pragma omp parallel
                    for(auto i = 0ul; i < N; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = N; j < basis_size; ++j) {
                            results[thread] +=
                                expectation_value_1_3(basis[i], basis[j]) / 2.
                                * types::add_conjugate_products(vector[i], vector[j]);
                        }
                    }

#pragma omp parallel
                    for(auto i = N; i < basis_size; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = N; j < basis_size; ++j) {
                            results[thread] +=
                                expectation_value_3_3(basis[i], basis[j]) / 2.
                                * types::multiply_with_conjugate(vector[i], vector[j]);
                        }
                    }

                    const auto result =
                        std::accumulate(results.begin(), results.end(), std::complex<Float>(0.));

                    assert(result.imag() < 1e-15);

                    return result.real();
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
