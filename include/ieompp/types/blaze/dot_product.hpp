#ifndef IEOMPP_TYPES_BLAZE_DOT_PRODUCT_HPP_
#define IEOMPP_TYPES_BLAZE_DOT_PRODUCT_HPP_

#include "ieompp/types/blaze/traits.hpp"

namespace ieompp
{
    namespace types
    {
        template <typename A, typename B>
        typename std::enable_if<!std::is_arithmetic<A>::value && !std::is_arithmetic<B>::value,
                                typename A::ElementType>::type
        dot_product(const A& a, const B& b)
        {
            return blaze::trans(a) * b;
        }
    } // namespace types
} // namespace ieompp

#endif
