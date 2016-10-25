#ifndef IEOMPP_TYPES_NUMBER_HPP_
#define IEOMPP_TYPES_NUMBER_HPP_

#include <cmath>
#include <complex>
#include <limits>
#include <type_traits>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        struct IsComplex {
            static constexpr bool value = false;
        };

        template <typename T>
        struct IsComplex<std::complex<T>> {
            static constexpr bool value = true;
        };

        namespace detail
        {
            template <typename T, bool is_c>
            struct RealTypeHelper;

            template <typename T>
            struct RealTypeHelper<T, false> {
                using type = T;
            };

            template <typename T>
            struct RealTypeHelper<T, true> {
                using type = typename T::value_type;
            };
        } // namespace detail

        template <typename T>
        struct RealType {
            using type = typename detail::RealTypeHelper<T, IsComplex<T>::value>::type;
        };

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
            static const auto epsilon = std::nextafter(std::numeric_limits<T>::min(), 1.);
            return std::abs(t1 - t2) < epsilon;
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, bool>::type IsEqual(const T& t1, const T& t2)
        {
            return IsEqual(t1.real(), t2.real()) && IsEqual(t1.imag(), t2.imag());
        }

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, bool>::type IsZero(const T& t)
        {
            return t == static_cast<T>(0);
        }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type IsZero(const T& t)
        {
            static const auto epsilon = std::nextafter(std::numeric_limits<T>::min(), 1.);
            return std::abs(t) < epsilon;
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, bool>::type IsZero(const T& t)
        {
            using Float = typename RealType<T>::type;
            return IsZero<Float>(t.real()) && IsZero<Float>(t.imag());
        }

        template <typename T>
        typename std::enable_if<IsComplex<T>::value, T>::type conjugate(T t)
        {
            t.imag(-t.imag());
            return t;
        }

        template <typename T>
        typename std::enable_if<!IsComplex<T>::value, T>::type conjugate(T t)
        {
            return t;
        }
    } // namespace types
} // namespace ieompp

#endif
