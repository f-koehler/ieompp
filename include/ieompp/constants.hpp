#ifndef IEOMPP_CONSTANTS_HPP_
#define IEOMPP_CONSTANTS_HPP_

#ifdef IEOMPP_HAS_BOOST
#include <boost/math/constants/constants.hpp>
#endif

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


#ifdef IEOMPP_HAS_BOOST
    template <typename Real>
    const Real Pi<Real>::value = boost::math::constants::pi<Real>();

    template <typename Real>
    const Real HalfPi<Real>::value = boost::math::constants::half_pi<Real>();

    template <typename Real>
    const Real TwoPi<Real>::value = boost::math::constants::two_pi<Real>();
#else
    // values were calculated using boost::math::constants from Boost 1.60.0
    template <>
    const float Pi<float>::value = 3.1415927410125732421875f;

    template <>
    const double Pi<double>::value = 3.141592653589793115997963468544185161590576171875;

    template <>
    const long double Pi<long double>::value =
        3.14159265358979323851280895940618620443274267017841339111328125l;


    template <>
    const float HalfPi<float>::value = 1.57079637050628662109375f;

    template <>
    const double HalfPi<double>::value = 1.5707963267948965579989817342720925807952880859375;

    template <>
    const long double HalfPi<long double>::value =
        1.570796326794896619256404479703093102216371335089206695556640625l;


    template <>
    const float TwoPi<float>::value = 6.283185482025146484375;

    template <>
    const double TwoPi<double>::value = 6.28318530717958623199592693708837032318115234375;

    template <>
    const long double TwoPi<long double>::value =
        6.2831853071795864770256179188123724088654853403568267822265625l;
#endif
}

#endif
