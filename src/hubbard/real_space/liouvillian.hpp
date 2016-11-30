#ifndef LIOUVILLIAN_HPP_
#define LIOUVILLIAN_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/liouvillian.hpp>

auto init_liouvillian(const double J)
{
    get_loggers().main->info("Set up Liouvillian with:");
    get_loggers().main->info("\tJ = {}", J);
    auto L = ieompp::models::hubbard_real_space::make_liouvillian(J, 0.);
    get_loggers().main->info("Finished setting up Liouvillian");
    return L;
}

auto init_liouvillian(const double J, const double U)
{
    get_loggers().main->info("Set up Liouvillian with:");
    get_loggers().main->info("\tJ = {}", J);
    get_loggers().main->info("\tU = {}", U);
    auto L = ieompp::models::hubbard_real_space::make_liouvillian(J, U);
    get_loggers().main->info("Finished setting up Liouvillian");
    return L;
}

#endif
