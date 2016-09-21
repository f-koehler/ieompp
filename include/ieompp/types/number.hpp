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
        struct is_complex {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_complex<std::complex<T>> {
            static constexpr bool value = true;
        };

        namespace detail
        {
            template <typename T, bool is_c>
            struct real_type_helper;

            template <typename T>
            struct real_type_helper<T, false> {
                using type = T;
            };

            template <typename T>
            struct real_type_helper<T, true> {
                using type = typename T::value_type;
            };
        }

        template <typename T>
        struct real_type {
            using type = typename detail::real_type_helper<T, is_complex<T>::value>::type;
        };

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, bool>::type is_zero(const T& c)
        {
            static const auto epsilon  = std::nextafter(std::numeric_limits<T>::min(), 1.);
            static const auto epsilon2 = -epsilon;
            return (c < epsilon) && (c > epsilon2);
        }

        template <typename T>
        typename std::enable_if<is_complex<T>::value, bool>::type is_zero(const T& c)
        {
            using Real = typename real_type<T>::type;
            return is_zero<Real>(c.real()) && is_zero<Real>(c.imag());
        }

        template <typename T>
        typename std::enable_if<is_complex<T>::value, T>::type conjugate(T t)
        {
            t.imag(-t.imag());
            return t;
        }

        template <typename T>
        typename std::enable_if<!is_complex<T>::value, T>::type conjugate(T t)
        {
            return t;
        }
    }
}

#endif
