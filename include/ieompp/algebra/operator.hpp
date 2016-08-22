#ifndef IEOMPP_ALGEBRA_OPERATOR_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_HPP_

#include <ieompp/algebra/operator/get_index.hpp>
#include <ieompp/algebra/operator/io.hpp>
#include <ieompp/algebra/operator/make.hpp>
#include <ieompp/algebra/operator/operator.hpp>

namespace ieompp
{
    namespace algebra
    {
        namespace detail
        {
            template <typename Operator, std::size_t I, std::size_t N>
            struct has_symbolic_index_helper {
                static constexpr bool value =
                    !std::is_fundamental<typename index_type<I, Operator>::type>::value
                    || has_symbolic_index_helper<Operator, I + 1, N>::value;
            };

            template <typename Operator, std::size_t N>
            struct has_symbolic_index_helper<Operator, N, N> {
                static constexpr bool value =
                    !std::is_fundamental<typename index_type<N, Operator>::type>::value;
            };
        }

        template <typename T>
        struct is_operator {
            static constexpr bool value = false;
        };

        template <typename... Ts>
        struct is_operator<Operator<Ts...>> {
            static constexpr bool value = true;
        };

        template <typename Operator>
        struct has_symbolic_index {
            static constexpr bool value =
                detail::has_symbolic_index_helper<Operator, 0,
                                                  Operator::number_of_indices - 1>::value;
        };
    }
}

#endif
