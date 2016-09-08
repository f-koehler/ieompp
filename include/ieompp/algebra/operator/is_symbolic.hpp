#ifndef IEOMPP_ALGEBRA_IS_SYMBOLIC_HPP_
#define IEOMPP_ALGEBRA_IS_SYMBOLIC_HPP_

#include <ieompp/algebra/operator/get_index.hpp>

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Op, std::size_t I, std::size_t N>
            struct is_symbolic_operator_impl {
                static constexpr bool value =
                    !std::is_arithmetic<typename index_type<I, Op>::type>::value
                    || is_symbolic_operator_impl<Op, I + 1, N>::value;
            };

            template <typename Op, std::size_t N>
            struct is_symbolic_operator_impl<Op, N, N> {
                static constexpr bool value =
                    !std::is_arithmetic<typename index_type<N, Op>::type>::value;
            };
        }

        template <typename Op>
        struct is_symbolic_operator {
            static constexpr bool value =
                detail::is_symbolic_operator_impl<Op, 0, Op::number_of_indices - 1>::value;
        };

        template <std::size_t I, typename Operator>
        struct is_symbolic_index {
            static constexpr bool value =
                !std::is_arithmetic<typename index_type<I, Operator>::type>::value;
        };
    }
}

#endif
