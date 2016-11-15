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

            template <typename Term>
            struct SiteOccupation<Basis1Operator<Term>> {
                using Basis    = Basis1Operator<Term>;
                using Float    = typename types::RealType<typename Term::Prefactor>::Type;
                using Operator = typename Term::Operator;
                using ExpectationValueFunction =
                    std::function<Float(const Operator&, const Operator&)>;

                ExpectationValueFunction expectation_value;

                template <typename Vector>
                Float operator()(const Basis& basis, const Vector& vector) const
                {
                    const auto size = basis.size();
                    std::vector<Float> results(omp_get_max_threads(), 0);

#pragma omp parallel for
                    for(auto i = 0ul; i < size; ++i) {
                        const auto thread = omp_get_thread_num();
                        for(auto j = 0ul; j < i; ++j) {
                            results[thread] +=
                                expectation_value(basis[i].operators.front(),
                                                  basis[j].operators.front())
                                * types::add_conjugate_products(vector[i], vector[j]);
                        }
                        results[thread] +=
                            std::norm(vector[i]) * expectation_value(basis[i].operators.front(),
                                                                     basis[i].operators.front());
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
                            results[thread] +=
                                expectation_value(basis[i].operators[0], basis[j].operators[0])
                                * types::add_conjugate_products(vector[i], vector[j]);
                        }
                        results[thread] +=
                            std::norm(vector[i])
                            * expectation_value(basis[i].operators[0], basis[i].operators[0]);
                    }

#pragma omp parallel for schedule(dynamic, 1)
                    for(auto i = 0ul; i < N; ++i) {
                        const auto thread = omp_get_thread_num();
                        const auto& op_a  = basis[i].operators[0];
                        for(auto j = N; j < basis_size; ++j) {
                            const auto& ops_b = basis[j].operators;

                            if(ops_b[1].index1 == ops_b[2].index1) {
                                results[thread] +=
                                    2 * expectation_value(op_a, ops_b[0])
                                    * (expectation_value(ops_b[2], ops_b[1]) - .5)
                                    * types::add_conjugate_products(vector[i], vector[j]);
                            } else {
                                results[thread] +=
                                    2 * expectation_value(op_a, ops_b[0])
                                    * expectation_value(ops_b[2], ops_b[1])
                                    * types::add_conjugate_products(vector[i], vector[j]);
                            }
                        }
                    }

#pragma omp parallel for schedule(dynamic, 1)
                    for(auto i = N; i < basis_size; ++i) {
                        const auto thread    = omp_get_thread_num();
                        const auto& ops_a    = basis[i].operators;
                        const bool a_special = (ops_a[1].index1 == ops_a[2].index1);
                        for(auto j = N; j < i; ++j) {
                            const auto& ops_b    = basis[j].operators;
                            const bool b_special = (ops_b[1].index1 == ops_b[2].index1);
                            const auto summand1  = 4 * expectation_value(ops_a[0], ops_b[0])
                                                  * expectation_value(ops_a[1], ops_a[2])
                                                  * expectation_value(ops_b[2], ops_b[1]);
                            const auto summand2 =
                                4 * expectation_value(ops_a[0], ops_b[0])
                                * expectation_value(ops_a[1], ops_b[1])
                                * (((ops_a[2].index1 == ops_b[2].index1) ? 1. : 0.)
                                   - expectation_value(ops_b[2], ops_a[2]));
                            const auto summand3 = a_special
                                                      ? -2 * expectation_value(ops_a[0], ops_b[0])
                                                            * expectation_value(ops_b[1], ops_b[2])
                                                      : 0.;
                            const auto summand4 = b_special
                                                      ? -2 * expectation_value(ops_a[0], ops_b[0])
                                                            * expectation_value(ops_a[1], ops_a[2])
                                                      : 0.;
                            const auto summand5 = (a_special && b_special)
                                                      ? expectation_value(ops_a[0], ops_b[0])
                                                      : 0.;
                            results[thread] +=
                                (summand1 + summand2 + summand3 + summand4 + summand5)
                                * types::add_conjugate_products(vector[i], vector[j]);
                        }
                        const auto summand1 = 4 * expectation_value(ops_a[0], ops_a[0])
                                              * expectation_value(ops_a[1], ops_a[2])
                                              * expectation_value(ops_a[2], ops_a[1]);
                        const auto summand2 = 4 * expectation_value(ops_a[0], ops_a[0])
                                              * expectation_value(ops_a[1], ops_a[1])
                                              * (1 - expectation_value(ops_a[2], ops_a[2]));
                        const auto summand3 = a_special
                                                  ? -4 * expectation_value(ops_a[0], ops_a[0])
                                                            * expectation_value(ops_a[1], ops_a[2])
                                                        + expectation_value(ops_a[0], ops_a[0])
                                                  : 0.;
                        results[thread] += (summand1 + summand2 + summand3) * std::norm(vector[i]);
                    }

                    return std::accumulate(results.begin(), results.end(), 0.);
                }
            };
        } // namespace hubbard_real_space
    }     // namespace models
} // namespace ieompp

#endif
