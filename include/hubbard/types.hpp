#ifndef HUBBARD_TYPES_HPP_
#define HUBBARD_TYPES_HPP_

#include <complex>
#include <limits>

namespace hubbard
{

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

    std::ostream& format(std::ostream& strm, const Complex& z) {
        auto r_zero = is_zero(z.real());
        auto i_zero = is_zero(z.imag());
        if(r_zero == (i_zero == true))
            return strm << '0';
        if(r_zero)
            return strm << z.imag() << 'i';
        if(i_zero)
            return strm << z.real();
        strm << '(' << z.real();
        if(z.imag() < 0)
            strm << '-';
        else
            strm << '+';
        strm << z.imag() << "i)";
        return strm;
    }
}

#endif
