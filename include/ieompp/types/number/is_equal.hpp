#ifndef IEOMPP_TYPES_NUMBER_IS_EQUAL_HPP_
#define IEOMPP_TYPES_NUMBER_IS_EQUAL_HPP_

#include "ieompp/types/number/complex.hpp"

#include <limits>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, bool>::type IsEqual(const T& t1,
                                                                                const T& t2)
        {
            return t1 == t2;
        }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type IsEqual(const T& t1,
                                                                                      const T& t2)
        {
            static constexpr auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(t1 - t2) < epsilon;
        }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type
        IsEqual(const T& t1, const T& t2, const T& max_dev)
        {
            static constexpr auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(t1 - t2) < (max_dev + epsilon);
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, bool>::type IsEqual(const T& t1, const T& t2)
        {
            using Float = typename RealType<T>::Type;
            return IsEqual<Float>(t1.real(), t2.real()) && IsEqual<Float>(t1.imag(), t2.imag());
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, bool>::type
        IsEqual(const T& t1, const T& t2, const typename RealType<T>::Type& max_dev)
        {
            using Float = typename RealType<T>::Type;
            return IsEqual<Float>(t1.real(), t2.real(), max_dev)
                   && IsEqual<Float>(t1.imag(), t2.imag(), max_dev);
        }
    } // namespace types
} // namespace ieompp

#endif
