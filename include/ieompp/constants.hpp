#ifndef IEOMPP_CONSTANTS_HPP_
#define IEOMPP_CONSTANTS_HPP_

#include <boost/math/constants/constants.hpp>

namespace ieompp
{
    template <typename Float>
    struct Pi {
        static const Float value;
    };

    template <typename Float>
    struct HalfPi {
        static const Float value;
    };

    template <typename Float>
    struct TwoPi {
        static const Float value;
    };


    template <typename Float>
    const Float Pi<Float>::value = boost::math::constants::pi<Float>();

    template <typename Float>
    const Float HalfPi<Float>::value = boost::math::constants::half_pi<Float>();

    template <typename Float>
    const Float TwoPi<Float>::value = boost::math::constants::two_pi<Float>();
}

#endif
