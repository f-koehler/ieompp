#ifndef IEOMPP_TYPES_BLAZE_DOT_PRODUCT_HPP_
#define IEOMPP_TYPES_BLAZE_DOT_PRODUCT_HPP_

#include "ieompp/types/blaze/traits.hpp"

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<ieompp::types::IsBlazeVector<T>::value,
                                typename ieompp::types::ScalarType<T>::type>::type
        dot_product(const T& a, const T& b)
        {
            return blaze::trans(a) * b;
        }
    } // namespace types
} // namespace ieompp

#endif
