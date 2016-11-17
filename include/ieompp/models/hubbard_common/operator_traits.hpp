#ifndef IEOMPP_MODELS_HUBBARD_COMMON_OPERATOR_TRAITS_HPP_
#define IEOMPP_MODELS_HUBBARD_COMMON_OPERATOR_TRAITS_HPP_

#include "ieompp/algebra/operator/get_index.hpp"

#include <type_traits>

namespace ieompp
{
    namespace models
    {
        namespace hubbard_common
        {
            namespace detail
            {
                template <typename Operator, std::size_t NumberOfIndices>
                struct Is1dHubbardOperatorHelper {
                    static constexpr bool value = false;
                };

                template <typename Operator>
                struct Is1dHubbardOperatorHelper<Operator, 2> {
                    static constexpr bool value =
                        std::is_integral<typename Operator::Index1>::value
                        && std::is_same<typename Operator::Index2, bool>::value;
                };

                template <typename Operator, std::size_t NumberOfIndices>
                struct Is2dHubbardOperatorHelper {
                    static constexpr bool value = false;
                };

                template <typename Operator>
                struct Is2dHubbardOperatorHelper<Operator, 3> {
                    static constexpr bool value =
                        std::is_integral<typename algebra::IndexType<0, Operator>::Type>::value
                        && std::is_integral<typename algebra::IndexType<1, Operator>::Type>::value
                        && std::is_same<typename algebra::IndexType<2, Operator>::Type,
                                        bool>::value;
                };
            } // namespace detail

            template <typename Operator>
            struct Is1dHubbardOperator {
                static constexpr bool value =
                    detail::Is1dHubbardOperatorHelper<Operator, Operator::number_of_indices>::value;
            };

            template <typename Operator>
            struct Is2dHubbardOperator {
                static constexpr bool value =
                    detail::Is2dHubbardOperatorHelper<Operator, Operator::number_of_indices>::value;
            };

            template <typename Operator>
            struct IsHubbardOperator {
                static constexpr bool value =
                    Is1dHubbardOperator<Operator>::value || Is2dHubbardOperator<Operator>::value;
            };

            template <typename Operator>
            struct SpatialIndexType {
                static_assert(IsHubbardOperator<Operator>::value,
                              "Operator must be of 1d or 2d Hubbard type!");

                using Type = typename std::
                    conditional<Is1dHubbardOperator<Operator>::value, typename Operator::Index1,
                                std::pair<typename algebra::IndexType<0, Operator>::Type,
                                          typename algebra::IndexType<1, Operator>::Type>>::type;
            };
        } // namespace hubbard_common
    }     // namespace models
} // namespace ieompp

#endif
