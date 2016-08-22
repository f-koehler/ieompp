#ifndef IEOMPP_ALGEBRA_OPERATOR_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_HPP_

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
            struct has_index_of_type_helper {
                static constexpr bool value =
                    std::is_same<typename index_type<I, Op>::type, T>::value
                    || has_index_of_type_helper<Op, T, I + 1, N>::value;
            };

            template <typename Op, typename T, std::size_t N>
            struct has_index_of_type_helper<Op, T, N, N> {
                static constexpr bool value =
                    std::is_same<typename index_type<N, Op>::type, T>::value;
            };
        }

        template <typename Op, typename T>
        struct has_index_of_type
        {
            static_assert(is_operator<Op>::value, "Operator must be an operator");

            static constexpr bool value =
                detail::has_index_of_type_helper<Op, T, 0, Op::number_of_indices>::value;
        };
    }
}

#endif
