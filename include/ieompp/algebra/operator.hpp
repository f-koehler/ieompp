#ifndef IEOMPP_ALGEBRA_OPERATOR_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_HPP_

#include <ieompp/algebra/operator/description.hpp>
#include <ieompp/algebra/operator/get_index.hpp>
#include <ieompp/algebra/operator/io.hpp>
#include <ieompp/algebra/operator/make.hpp>
#include <ieompp/algebra/operator/operator.hpp>
#include <ieompp/algebra/operator/order.hpp>

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Op, typename T, std::size_t I, std::size_t N>
            struct HasIndexOfTypeHelper {
                static constexpr bool value =
                    std::is_same<typename IndexType<I, Op>::type, T>::value
                    || HasIndexOfTypeHelper<Op, T, I + 1, N>::value;
            };

            template <typename Op, typename T, std::size_t N>
            struct HasIndexOfTypeHelper<Op, T, N, N> {
                static constexpr bool value =
                    std::is_same<typename IndexType<N, Op>::type, T>::value;
            };
        }

        template <typename Op, typename T>
        struct HasIndexOfType
        {
            static_assert(IsOperator<Op>::value, "Operator must be an operator");

            static constexpr bool value =
                detail::HasIndexOfTypeHelper<Op, T, 0, Op::number_of_indices>::value;
        };
    }
}

#endif
