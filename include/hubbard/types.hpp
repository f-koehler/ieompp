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

    std::ostream& operator<<(std::ostream& strm, Complex rhs)
    {
        strm << "(" << rhs.real();
        if(rhs.imag() < 0.)
            strm << "-";
        else
            strm << "+";
        strm << rhs.imag() << "i";
        return strm;
    }
}

#endif
