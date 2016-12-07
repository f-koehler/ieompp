#ifndef IEOMPP_TYPES_NUMBER_IS_ZERO_HPP_
#define IEOMPP_TYPES_NUMBER_IS_ZERO_HPP_

#include "ieompp/types/number/complex.hpp"

#include <limits>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, bool>::type IsZero(const T& t)
        {
            return t == static_cast<T>(0);
        }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type IsZero(const T& t)
        {
            static constexpr auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(t) < epsilon;
        }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type
        IsZero(const T& t, const T& max_dev)
        {
            static constexpr auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(t) < (max_dev + epsilon);
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, bool>::type IsZero(const T& t)
        {
            using Float = typename RealType<T>::Type;
            return IsZero<Float>(t.real()) && IsZero<Float>(t.imag());
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, bool>::type
        IsZero(const T& t, const typename RealType<T>::Type& max_dev)
        {
            using Float = typename RealType<T>::Type;
            return IsZero<Float>(t.real(), max_dev) && IsZero<Float>(t.imag(), max_dev);
        }
    } // namespace types
} // namespace ieompp

#endif
