#ifndef IEOMPP_CONSTANTS_HPP_
#define IEOMPP_CONSTANTS_HPP_

#include <boost/math/constants/constants.hpp>

namespace ieompp
{
    template <typename Real>
    struct Pi {
        static const Real value;
    };

    template <typename Real>
    struct HalfPi {
        static const Real value;
    };

    template <typename Real>
    struct TwoPi {
        static const Real value;
    };


    template <typename Real>
    const Real Pi<Real>::value = boost::math::constants::pi<Real>();

    template <typename Real>
    const Real HalfPi<Real>::value = boost::math::constants::half_pi<Real>();

    template <typename Real>
    const Real TwoPi<Real>::value = boost::math::constants::two_pi<Real>();
}

#endif
