#ifndef IEOMPP_ALGEBRA_COMMUTATOR_HPP_
#define IEOMPP_ALGEBRA_COMMUTATOR_HPP_

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <vector>

#include <ieompp/algebra/anticommutator.hpp>
#include <ieompp/algebra/operator.hpp>

namespace ieompp
{
    namespace algebra
    {
        template <typename Term, typename Container>
        void commutate(const Term& a, const Term& b, Container& c)
        {
            /* static_assert(std::is_same<typename Term::Prefactor, typename
             * Term2::Prefactor>::value, */
            /*               "Both term types must have the same prefactor type"); */
            /* static_assert(std::is_same<typename Term::Operator, typename Term2::Operator>::value,
             */
            /*               "Both term types must have the same operator type"); */
            static_assert(std::is_same<typename Container::value_type, Term>::value,
                          "Container type must hold values of the same type as Term");

            auto& a_ops = a.operators;
            auto& b_ops = b.operators;
            auto n      = a_ops.size();
            auto m      = b_ops.size();

            assert(m * n % 2 == 0);

            const auto prefactor = a.prefactor * b.prefactor;
            for(std::size_t k = 1; k <= n; ++k) {
                for(std::size_t l = 1; l <= m; ++l) {
                    if(anticommutates(a_ops[k - 1], b_ops[l - 1])) continue;

                    const auto sign = (m * (n - k) + l - 1) % 2 ? -1 : 1;

                    Term&& new_term    = Term();
                    new_term.prefactor = prefactor;
                    new_term.prefactor *= sign;
                    /* if(is_symbolic_operator<typename Term::Operator>::value) */
                    /*     new_term.prefactor *= anticommutator(a_ops[k - 1], b_ops[l - 1]); */

                    auto& new_ops = new_term.operators;
                    if(k > 1)
                        std::copy(a_ops.begin(), a_ops.begin() + (k - 1),
                                  std::back_inserter(new_ops));
                    if(l > 1)
                        std::copy(b_ops.begin(), b_ops.begin() + (l - 1),
                                  std::back_inserter(new_ops));
                    std::copy(b_ops.begin() + l, b_ops.end(), std::back_inserter(new_ops));
                    std::copy(a_ops.begin() + k, a_ops.end(), std::back_inserter(new_ops));

                    c.emplace_back(new_term);
                }
            }
        }
    }
}

#endif
