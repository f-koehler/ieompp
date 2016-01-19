#ifndef HUBBARD_TYPES_HPP_
#define HUBBARD_TYPES_HPP_

#include <complex>
#include <limits>

namespace hubbard
{
    const auto PI = 3.14159265358979323846;

    using Real    = double;
    using Complex = std::complex<Real>;

    inline bool is_zero(const Real& x)
    {
        static const auto min = std::numeric_limits<Real>::min();
        return std::abs(x) <= min;
    }

    inline bool is_zero(const Complex& z)
    {
        static const auto min = std::numeric_limits<Real>::min();
        return (std::abs(z.real()) <= min) && (std::abs(z.imag()) <= min);
    }

    template <typename T>
    std::ostream& format(std::ostream& strm, const T& t);

    template <>
    std::ostream& format<Complex>(std::ostream& strm, const Complex& t)
    {
        auto r_zero = is_zero(t.real());
        auto i_zero = is_zero(t.imag());
        strm << '(';
        if(r_zero && i_zero) return strm << "0)";
        if(r_zero) return strm << t.imag() << "i)";
        if(i_zero) return strm << t.real() << ')';
        strm << t.real();
        if(t.imag() < 0)
            strm << '-';
        else
            strm << '+';
        strm << std::abs(t.imag()) << "i)";
        return strm;
    }
}

#endif
