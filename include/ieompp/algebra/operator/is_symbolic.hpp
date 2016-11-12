#ifndef IEOMPP_ALGEBRA_OPERATOR_IS_SYMBOLIC_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_IS_SYMBOLIC_HPP_

#include "ieompp/algebra/operator/get_index.hpp"

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Op, std::size_t I, std::size_t N>
            struct IsSymbolicOperatorHelper {
                static constexpr bool value =
                    !std::is_arithmetic<typename IndexType<I, Op>::Type>::value
                    || IsSymbolicOperatorHelper<Op, I + 1, N>::value;
            };

            template <typename Op, std::size_t N>
            struct IsSymbolicOperatorHelper<Op, N, N> {
                static constexpr bool value =
                    !std::is_arithmetic<typename IndexType<N, Op>::Type>::value;
            };
        } // namespace detail

        template <typename Op>
        struct IsSymbolicOperator {
            static constexpr bool value =
                detail::IsSymbolicOperatorHelper<Op, 0, Op::number_of_indices - 1>::value;
        };

        template <std::size_t I, typename Operator>
        struct IsSymbolicIndex {
            static constexpr bool value =
                !std::is_arithmetic<typename IndexType<I, Operator>::Type>::value;
        };
    } // namespace algebra
} // namespace ieompp

#endif
