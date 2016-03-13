#ifndef HUBBARD_TYPES_HPP_
#define HUBBARD_TYPES_HPP_

#include <complex>
#include <limits>
#include <type_traits>

namespace hubbard
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

    /* template <> */
    /* std::ostream& format<Complex>(std::ostream& strm, const Complex& t) */
    /* { */
    /*     auto r_zero = is_zero(t.real()); */
    /*     auto i_zero = is_zero(t.imag()); */
    /*     strm << '('; */
    /*     if(r_zero && i_zero) return strm << "0)"; */
    /*     if(r_zero) return strm << t.imag() << "i)"; */
    /*     if(i_zero) return strm << t.real() << ')'; */
    /*     strm << t.real(); */
    /*     if(t.imag() < 0) */
    /*         strm << '-'; */
    /*     else */
    /*         strm << '+'; */
    /*     strm << std::abs(t.imag()) << "i)"; */
    /*     return strm; */
    /* } */
}

#endif
