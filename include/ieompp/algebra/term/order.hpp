#ifndef IEOMPP_ALGEBRA_TERM_ORDER_HPP_
#define IEOMPP_ALGEBRA_TERM_ORDER_HPP_

#include <ieompp/algebra/operator/order.hpp>

namespace ieompp
{
    namespace algebra
    {
        template <typename Term>
        struct TermSmaller {
            bool operator()(const Term& a, const Term& b) const
            {
                static const OperatorSmaller<typename Term::Operator> op_smaller{};

                auto size_a = a.operators.size();
                auto size_b = b.operators.size();
                if(size_a != size_b) {
                    return size_a < size_b;
                }
                for(auto it_a = a.operators.begin(), it_b = b.operators.begin();
                    it_a != a.operators.end(); ++it_a, ++it_b) {
                    if(*it_a != *it_b) {
                        return op_smaller(*it_a, *it_b);
                    }
                }
                return false;
            }
        };

        template <typename Term>
        struct TermGreater {
            bool operator()(const Term& a, const Term& b) const
            {
                static const OperatorGreater<typename Term::Operator> op_greater{};

                auto size_a = a.operators.size();
                auto size_b = b.operators.size();
                if(size_a != size_b) {
                    return size_a > size_b;
                }
                for(auto it_a = a.operators.begin(), it_b = b.operators.begin();
                    it_a != a.operators.end(); ++it_a, ++it_b) {
                    if(*it_a != *it_b) {
                        return op_greater(*it_a, *it_b);
                    }
                }
                return false;
            }
        };
    } // namespace algebra
} // namespace ieompp

#endif
