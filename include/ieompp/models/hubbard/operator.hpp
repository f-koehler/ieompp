#ifndef IEOMPP_MODELS_HUBBARD_OPERATOR_HPP_
#define IEOMPP_MODELS_HUBBARD_OPERATOR_HPP_

#include <type_traits>

namespace ieompp
{
    namespace hubbard
    {
        template <typename Operator>
        struct is_hubbard_operator {
            static constexpr bool value = (Operator::number_of_indices == 2)
                                          && std::is_integral<typename Operator::Index1>::value
                                          && std::is_same<typename Operator::Index2, bool>::value;
        };
    } // namespace hubbard
} // namespace ieompp

#endif
