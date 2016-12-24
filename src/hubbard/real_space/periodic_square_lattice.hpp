#ifndef PERIODIC_SQUARE_LATTICE_HPP_
#define PERIODIC_SQUARE_LATTICE_HPP_

#include "../include/logging.hpp"

#include <ieompp/lattices/periodic_square_lattice.hpp>

auto init_lattice(uint64_t Nx, uint64_t Ny, double dx = 1., double dy = 1.)
{
    get_loggers().main->info("Init periodic square lattice with N = {} sites");
    ieompp::lattices::PeriodicSquareLattice<double> chain(Nx, Ny, dx, dy);
    get_loggers().main->info("Finished setting up lattice");
    return chain;
}

#endif
