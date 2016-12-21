#ifndef EXPECTATION_VALUE_HPP_
#define EXPECTATION_VALUE_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/expectation_value.hpp>

template <typename Lattice, typename Float>
auto init_expectation_value(const Lattice& lattice, const Float& J, const Float& filling_factor)
{
    get_loggers().main->info("Start to precalculate expectation values");
    ieompp::models::hubbard_real_space::ExpectationValue1DHalfFilled<double, Lattice> ev(lattice, J, filling_factor);
    get_loggers().main->info("Finish precalculating expectation values");
    return ev;
}

#endif
