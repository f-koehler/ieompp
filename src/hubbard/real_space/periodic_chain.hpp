#ifndef PERIODIC_CHAIN_HPP_
#define PERIODIC_CHAIN_HPP_

#include "../include/logging.hpp"

#include <ieompp/lattices/periodic_chain.hpp>

auto init_lattice(uint64_t N, uint64_t dx = 1.)
{
    get_loggers().main->info("Init periodic chain with N = {} sites");
    ieompp::lattices::PeriodicChain<double> chain(N, dx);
    get_loggers().main->info("Finished setting up lattice");
    return chain;
}

#endif
