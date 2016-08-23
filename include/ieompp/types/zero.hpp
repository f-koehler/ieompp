#ifndef IEOMPP_TYPES_ZERO_HPP_
#define IEOMPP_TYPES_ZERO_HPP_

#include <type_traits>

#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<ieompp::types::is_eigen_matrix<T>::value, T>::type
        zero(const T& templ)
        {
            return T::Zero(templ.rows(), templ.cols());
        }

        template <typename T>
        typename std::enable_if<!ieompp::types::is_eigen_matrix<T>::value, T>::type
        zero(const T& templ)
        {
            (void)templ;
            return T(0.);
        }
    }
}

#endif
