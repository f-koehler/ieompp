#ifndef TESTS_UTIL_HPP_
#define TESTS_UTIL_HPP_

#include <complex>
#include <sstream>
#include <string>

#include "catch.hpp"

namespace ieompp
{
    template <typename Real>
    struct ApproxComplex {
        struct ResultType {
            bool value;
            std::string message;
            operator bool() const { return value; }
        };

        std::complex<Real> number;

        ResultType operator==(const std::complex<Real>& rhs) const
        {
            bool value =
                (Approx(number.real()) == rhs.real()) && (Approx(number.imag()) == rhs.imag());
            std::ostringstream strm;
            strm << number << (value ? " == " : " != ") << rhs;
            return ResultType{value, strm.str()};
        }
    };

    template <typename Real>
    std::ostream& operator<<(std::ostream& strm, typename ApproxComplex<Real>::ResultType& rhs)
    {
        strm << rhs.message;
        return strm;
    }
}

#endif
