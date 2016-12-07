#ifndef IEOMPP_TYPES_NUMBER_COMPLEX_HPP_
#define IEOMPP_TYPES_NUMBER_COMPLEX_HPP_

#include <complex>

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
                using Type = T;
            };

            template <typename T>
            struct RealTypeHelper<T, true> {
                using Type = typename T::value_type;
            };
        } // namespace detail

        template <typename T>
        struct RealType {
            using Type = typename detail::RealTypeHelper<T, IsComplex<T>::value>::Type;
        };

        // TODO: this should be called get_conjugate
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

        template <typename Scalar>
        std::complex<Scalar> multiply_with_conjugate(const std::complex<Scalar>& a,
                                                     const std::complex<Scalar>& b)
        {
            return std::complex<Scalar>(a.real() * b.real() + a.imag() * b.imag(),
                                        -a.real() * b.imag() + b.real() * a.imag());
        }

        template <typename Scalar>
        Scalar add_conjugate_products(const std::complex<Scalar>& a, const std::complex<Scalar>& b)
        {
            return 2 * (a.real() * b.real() + a.imag() * b.imag());
        }
    } // namespace types
} // namespace ieompp

#endif
