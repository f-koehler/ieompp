#ifndef IEOMPP_ALGEBRA_OPERATOR_ORDER_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_ORDER_HPP_

#include <ieompp/algebra/operator/get_index.hpp>
#include <ieompp/algebra/operator/operator.hpp>

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Operator, std::size_t I, std::size_t N>
            struct OperatorComparisonHelperRec {
                static bool smaller(const Operator& a, const Operator& b)
                {
                    auto& a_idx = get_index<I>(a);
                    auto& b_idx = get_index<I>(b);
                    if(a_idx != b_idx) return a_idx < b_idx;
                    return OperatorComparisonHelperRec<Operator, I + 1, N>::smaller(a, b);
                }

                static bool greater(const Operator& a, const Operator& b)
                {
                    auto& a_idx = get_index<I>(a);
                    auto& b_idx = get_index<I>(b);
                    if(a_idx != b_idx) return a_idx > b_idx;
                    return OperatorComparisonHelperRec<Operator, I + 1, N>::greater(a, b);
                }
            };

            template <typename Operator, std::size_t N>
            struct OperatorComparisonHelperRec<Operator, N, N> {
                static bool smaller(const Operator& a, const Operator& b)
                {
                    (void)a;
                    (void)b;
                    return false;
                }
                static bool greater(const Operator& a, const Operator& b)
                {
                    (void)a;
                    (void)b;
                    return false;
                }
            };

            template <typename Operator, std::size_t N>
            struct OperatorComparisonHelper {
                static bool smaller(const Operator& a, const Operator& b)
                {
                    if(a.creator != b.creator) return a.creator;
                    return OperatorComparisonHelperRec<Operator, 0, N>::smaller(a, b);
                }
                static bool greater(const Operator& a, const Operator& b)
                {
                    if(a.creator != b.creator) return b.creator;
                    return OperatorComparisonHelperRec<Operator, 0, N>::greater(a, b);
                }
            };


            template <typename Operator>
            struct OperatorComparisonHelper<Operator, 1> {
                static bool smaller(const Operator& a, const Operator& b)
                {
                    if(a.creator != b.creator) return a.creator;
                    return a.index < b.index;
                }
                static bool greater(const Operator& a, const Operator& b)
                {
                    if(a.creator != b.creator) return b.creator;
                    return a.index > b.index;
                }
            };

            template <typename Operator>
            struct OperatorComparisonHelper<Operator, 2> {
                static bool smaller(const Operator& a, const Operator& b)
                {
                    if(a.creator != b.creator) return a.creator;
                    if(a.index1 != b.index1) return a.index1 < b.index1;
                    return a.index2 < b.index2;
                }
                static bool greater(const Operator& a, const Operator& b)
                {
                    if(a.creator != b.creator) return b.creator;
                    if(a.index1 != b.index1) return a.index1 > b.index1;
                    return a.index2 > b.index2;
                }
            };
        }

        template <typename Operator>
        struct OperatorSmaller {
            bool operator()(const Operator& a, const Operator& b) const
            {
                return detail::OperatorComparisonHelper<Operator,
                                                        Operator::number_of_indices>::smaller(a, b);
            }
        };

        template <typename Operator>
        struct OperatorGreater {
            bool operator()(const Operator& a, const Operator& b) const
            {
                return detail::OperatorComparisonHelper<Operator,
                                                        Operator::number_of_indices>::greater(a, b);
            }
        };
    }
}

#endif
