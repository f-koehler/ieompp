#ifndef IEOMPP_ODE_STEP_SIZE_CONTROLLER_HPP_
#define IEOMPP_ODE_STEP_SIZE_CONTROLLER_HPP_

#include <algorithm>
#include <cmath>

namespace ieompp
{
    namespace ode
    {
        template <typename Real>
        struct StepSizeController
        {
            Real safety_factor = 0.9;
            Real lower_limit   = 0.2;
            Real upper_limit   = 2.0;
            Real order         = 0.25;
            Real tolerance     = 1e-3;

            bool operator()(Real& step_size, const Real& error) const {
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
