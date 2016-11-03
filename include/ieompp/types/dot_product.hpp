#ifndef IEOMPP_TYPES_DOT_PRODUCT_HPP_
#define IEOMPP_TYPES_DOT_PRODUCT_HPP_

#include <type_traits>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, T>::type dot_product(const T& a,
                                                                                   const T& b)
        {
            return a * b;
        }
    } // namespace types
} // namespace ieompp

#endif
