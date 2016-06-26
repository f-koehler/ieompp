#ifndef IEOMPP_TYPES_COMPLEX_HPP_
#define IEOMPP_TYPES_COMPLEX_HPP_

#include <complex>

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
    }
}

#endif
