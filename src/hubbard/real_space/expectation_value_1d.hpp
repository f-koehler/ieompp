#ifndef EXPECTATION_VALUE_HPP_
#define EXPECTATION_VALUE_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard/dispersion.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>

template <typename Lattice, typename Float>
auto init_expectation_value(const Lattice& lattice, const Float& filling_factor)
{
    get_loggers().main->info("Start to precalculate expectation values");
    ieompp::models::hubbard_real_space::ExpectationValue1DHalfFilled<Float, Lattice> ev(
        lattice, filling_factor,
        ieompp::models::hubbard::calculate_fermi_momentum_1d(filling_factor));
    get_loggers().main->info("Finish precalculating expectation values");
    return ev;
}

#endif
