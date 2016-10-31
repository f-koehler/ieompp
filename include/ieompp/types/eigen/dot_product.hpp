#ifndef IEOMPP_TYPES_EIGEN_DOT_PRODUCT_HPP_
#define IEOMPP_TYPES_EIGEN_DOT_PRODUCT_HPP_

#include "ieompp/types/eigen/traits.hpp"

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<ieompp::types::IsEigenMatrix<T>::value,
                                typename ieompp::types::ScalarType<T>::type>::type
        dot_product(const T& a, const T& b)
        {
            return a.dot(b);
        }
    }
}

#endif
