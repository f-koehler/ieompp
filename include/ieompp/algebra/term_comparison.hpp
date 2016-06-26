#ifndef IEOMPP_ALGEBRA_TERM_COMPARISON_HPP_
#define IEOMPP_ALGEBRA_TERM_COMPARISON_HPP_

#include "ieompp/algebra/operator_comparison.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <typename Term>
        struct TermSmaller {
            bool operator()(const Term& a, const Term& b) const
            {
                static const OperatorSmaller<typename Term::Operator> op_smaller;
                static const OperatorGreater<typename Term::Operator> op_greater;

                auto size_a = a.operators.size();
                auto size_b = b.operators.size();
                if(size_a < size_b) return true;
                if(size_a > size_b) return false;
                for(auto it_a = a.operators.begin(), it_b = b.operators.end();
                    it_a != a.operators.end(); ++it_a, ++it_b) {
                    if(op_smaller(*it_a, *it_b)) return true;
                    if(op_greater(*it_a, *it_b)) return false;
                }
                return false;
            }
        };

        template <typename Term>
        struct TermGreater {
            bool operator()(const Term& a, const Term& b) const
            {
                static const OperatorSmaller<typename Term::Operator> op_smaller;
                static const OperatorGreater<typename Term::Operator> op_greater;

                auto size_a = a.operators.size();
                auto size_b = b.operators.size();
                if(size_a > size_b) return true;
                if(size_a < size_b) return false;
                for(auto it_a = a.operators.begin(), it_b = b.operators.end();
                    it_a != a.operators.end(); ++it_a, ++it_b) {
                    if(op_smaller(*it_a, *it_b)) return false;
                    if(op_greater(*it_a, *it_b)) return true;
                }
                return false;
            }
        };
    }
}

#endif
