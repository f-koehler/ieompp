#ifndef IEOMPP_ALGEBRA_TERM_LIST_HPP_
#define IEOMPP_ALGEBRA_TERM_LIST_HPP_

#include "ieompp/algebra/term/order.hpp"

#include <algorithm>
#include <functional>
#include <list>

namespace ieompp
{
    namespace algebra
    {
        template <typename ContainerA, typename ContainerB>
        void copy_as_unique_terms(const ContainerA& original, ContainerB& result)
        {
            using Term = typename ContainerA::value_type;

            algebra::TermSmaller<Term> smaller;
            std::list<Term> lst;
            for(auto& t : original) {
                auto pos = std::lower_bound(lst.begin(), lst.end(), t, smaller);
                if(pos == lst.end()) {
                    lst.push_back(t);
                    continue;
                }
                if(pos->same_operators(t)) {
                    pos->prefactor += t.prefactor;
                } else {
                    lst.insert(pos, t);
                }
            }
            std::copy(lst.begin(), lst.end(), std::back_inserter(result));
        }
    } // namespace algebra
} // namespace ieompp

#endif
