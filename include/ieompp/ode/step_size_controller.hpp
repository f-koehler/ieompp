#ifndef IEOMPP_ODE_STEP_SIZE_CONTROLLER_HPP_
#define IEOMPP_ODE_STEP_SIZE_CONTROLLER_HPP_

#include <algorithm>
#include <cmath>

namespace ieompp
{
    namespace ode
    {
        template <typename Float>
        struct StepSizeController
        {
            Float safety_factor = 0.9;
            Float lower_limit   = 0.2;
            Float upper_limit   = 2.0;
            Float order         = 0.25;
            Float tolerance     = 1e-3;

            bool operator()(Float& step_size, const Float& error) const {
                bool retval = error > tolerance;
                step_size *= std::min(
                    std::max(safety_factor * std::pow(tolerance / error, order), lower_limit),
                    upper_limit);
                return retval;
            }
        };
    }
}


#endif
