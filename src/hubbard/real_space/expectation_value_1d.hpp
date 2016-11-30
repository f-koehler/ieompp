#ifndef EXPECTATION_VALUE_HPP_
#define EXPECTATION_VALUE_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/expectation_value.hpp>

template <typename Lattice>
auto init_expectation_value(const Lattice& lattice)
{
    get_loggers().main->info("Start to precalculate expectation values");
    ieompp::models::hubbard_real_space::ExpectationValue1DHalfFilled<double, Lattice> ev(lattice);
    get_loggers().main->info("Finish precalculating expectation values");
    return ev;
}

#endif
