#ifndef IEOMPP_MODELS_HUBBARD_REAL_SPACE_SITE_OCCUPATION_HPP_
#define IEOMPP_MODELS_HUBBARD_REAL_SPACE_SITE_OCCUPATION_HPP_

#include <functional>
#include <numeric>
#include <vector>

#include <omp.h>

#include "ieompp/models/hubbard_real_space/basis.hpp"
#include "ieompp/types/matrix.hpp"
#include "ieompp/types/number.hpp"

namespace ieompp
{
    namespace models
    {
        namespace hubbard_real_space
        {
            template <typename Basis>
            struct SiteOccupation {
            };

            template <typename TermT>
            struct SiteOccupation<Basis1Operator<TermT>> {
                using Basis    = Basis1Operator<TermT>;
                using Term     = TermT;
                using Float    = typename types::RealType<typename Term::Prefactor>::Type;
                using Operator = typename Term::Operator;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;

                ExpectationValueFunction expectation_value;

                Float expectation_value_1_1(const Term& a, const Term& b) const
                {
                    assert(a.operators.size() == 1);
                    assert(b.operators.size() == 1);

                    return 2. * expectation_value(a.operators.front(), b.operators.front());
                }

                template <typename Vector>
                Float operator()(const Basis& basis, const Vector& vector) const
                {
                    const auto size = basis.size();
                    std::vector<Float> results(omp_get_max_threads(), 0);

#pragma omp parallel for
                    for(auto i = 0ul; i < size; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = 0ul; j < i; ++j) {
                            results[thread] += expectation_value_1_1(basis[i], basis[j])
                                               * types::add_conjugate_products(vector[i], vector[j])
                                               / 2.;
                        }
                        results[thread] +=
                            std::norm(vector[i]) * expectation_value_1_1(basis[i], basis[i]) / 2.;
                    }

                    return std::accumulate(results.begin(), results.end(), 0.);
                }
            };

            template <typename Term>
            struct SiteOccupation<Basis3Operator<Term>> {
                using Basis    = Basis3Operator<Term>;
                using Float    = typename types::RealType<typename Term::Prefactor>::Type;
                using Operator = typename Term::Operator;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;

                ExpectationValueFunction expectation_value;

                Float expectation_value_1_1(const Term& a, const Term& b) const
                {
                    assert(a.operators.size() == 1);
                    assert(b.operators.size() == 1);

                    return 2. * expectation_value(a.operators.front(), b.operators.front());
                }

                Float expectation_value_1_3(const Term& a, const Term& b) const
                {
                    assert(a.operators.size() == 1);
                    assert(b.operators.size() == 3);

                    Float ret = expectation_value(b.operators.back(), b.operators[1]);
                    if(b.operators[1] == b.operators[2]) {
                        ret -= 0.5;
                    }
                    ret *= expectation_value(a.operators.front(), b.operators.front());
                    return 4 * ret;
                }

                Float expectation_value_3_3(const Term& a, const Term& b) const
                {
                    assert(a.operators.size() == 3);
                    assert(b.operators.size() == 3);

                    const auto& ops_a = a.operators;
                    const auto& ops_b = b.operators;

                    Float ev_6 = 8 * expectation_value(ops_a[0], ops_b[0])
                                 * expectation_value(ops_a[1], ops_b[1])
                                 * expectation_value(ops_b[1], ops_b[2]);
                    ev_6 += 8 * expectation_value(ops_a[0], ops_b[0])
                            * expectation_value(ops_a[1], ops_b[1])
                            * (((ops_a[2].index1 == ops_b[2].index1) ? 1. : 0.)
                               - expectation_value(ops_b[2], ops_a[2]));

                    Float ev_4 = 0.;
                    if(ops_b[1].index1 == ops_b[2].index1) {
                        ev_4 -= 4 * expectation_value(ops_a[0], ops_b[0])
                                * expectation_value(ops_a[1], ops_a[2]);
                    }
                    if(ops_a[1].index1 == ops_a[2].index1) {
                        ev_4 -= 4 * expectation_value(ops_a[0], ops_b[0])
                                * expectation_value(ops_b[1], ops_b[2]);
                    }

                    Float ev_2 = 0.;
                    if((ops_a[1].index1 == ops_a[2].index1)
                       && (ops_b[1].index1 == ops_b[2].index1)) {
                        ev_2 += 2 * expectation_value(ops_a[0], ops_b[0]);
                    }

                    return ev_6 + ev_4 + ev_2;
                }

                template <typename Vector>
                Float operator()(const Basis& basis, const Vector& vector) const
                {
                    const auto N          = basis.N;
                    const auto basis_size = basis.size();
                    std::vector<Float> results(omp_get_max_threads(), 0);

#pragma omp parallel for schedule(dynamic, 1)
                    for(auto i = 0ul; i < N; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = 0ul; j < i; ++j) {
                            results[thread] += expectation_value_1_1(basis[i], basis[j])
                                               * types::add_conjugate_products(vector[i], vector[j])
                                               / 2.;
                        }
                        results[thread] +=
                            std::norm(vector[i]) * expectation_value_1_1(basis[i], basis[i]) / 2.;
                    }

#pragma omp parallel for schedule(dynamic, 1)
                    for(auto i = 0ul; i < N; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = N; j < basis_size; ++j) {
                            results[thread] += expectation_value_1_3(basis[i], basis[j])
                                               * types::add_conjugate_products(vector[i], vector[j])
                                               / 2.;
                        }
                    }

#pragma omp parallel for schedule(dynamic, 1)
                    for(auto i = N; i < basis_size; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = N; j < i; ++j) {
                            results[thread] += expectation_value_3_3(basis[i], basis[j])
                                               * types::add_conjugate_products(vector[i], vector[j])
                                               / 2.;
                        }
                        results[thread] +=
                            expectation_value_3_3(basis[i], basis[i]) * std::norm(vector[i]) / 2.;
                    }

                    return std::accumulate(results.begin(), results.end(), 0.);
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
