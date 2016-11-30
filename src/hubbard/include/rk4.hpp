#ifndef RK4_HPP_
#define RK4_HPP_

#include "../include/logging.hpp"

#include <ieompp/ode/rk4.hpp>

template <typename Float>
ieompp::ode::RK4<Float> init_rk4(uint64_t basis_size, const Float& dt)
{
    get_loggers().ode->info("Init RK4 integrator");
    ieompp::ode::RK4<Float> rk4(basis_size, dt);
    get_loggers().ode->info("Finished RK4 initializing integrator");
    return rk4;
}

#endif
