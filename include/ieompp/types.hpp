#ifndef IEOMPP_TYPES_HPP_
#define IEOMPP_TYPES_HPP_

#include <complex>
#include <limits>
#include <type_traits>

namespace ieompp
{
    template <typename T>
    inline auto dot_product(const T& a, const T& b) -> decltype(a.dot(b))
    {
        return a.dot(b);
    }

    template <typename T>
    inline auto dot_product(const T& a, const T& b) -> decltype(a* b)
    {
        return a * b;
    }

    template <typename Real>
    inline bool is_zero(const Real& x)
    {
        static const auto min = std::numeric_limits<Real>::min();
        return std::abs(x) <= min;
    }

    template <typename Real>
    inline bool is_zero(const std::complex<Real>& z)
    {
        static const auto min = std::numeric_limits<Real>::min();
        return (std::abs(z.real()) <= min) && (std::abs(z.imag()) <= min);
    }
}

#endif
