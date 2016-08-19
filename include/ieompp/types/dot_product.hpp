#ifndef IEOMPP_TYPES_DOT_PRODUCT_HPP_
#define IEOMPP_TYPES_DOT_PRODUCT_HPP_

#include <type_traits>

#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<ieompp::types::is_eigen_matrix<T>::value,
                                typename ieompp::types::eigen_matrix_traits<T>::Scalar>::type
        dot_product(const T& a, const T& b)
        {
            return a.dot(b);
        }

        template <typename T>
        typename std::enable_if<!ieompp::types::is_eigen_matrix<T>::value, T>::type
        dot_product(const T& a, const T& b)
        {
            return a * b;
        }
    }
}

#endif
