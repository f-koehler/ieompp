#ifndef MOMENTUM_SPACE_1D_HPP_
#define MOMENTUM_SPACE_1D_HPP_

#include "momentum_space.hpp"

#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_momentum_space/basis.hpp>
#include <ieompp/models/hubbard_momentum_space/particle_number.hpp>

using Basis3        = ieompp::models::hubbard_momentum_space::Basis3Operator<Monomial>;
using Lattice       = ieompp::lattices::PeriodicChain<double, uint64_t>;
using BrillouinZone = ieompp::lattices::PeriodicChain<double, uint64_t>;

template <typename Basis>
auto init_basis(const BrillouinZone& brillouin_zone)
{
    get_loggers().main->info("Set up basis");
    auto basis = Basis(0, brillouin_zone);
    get_loggers().main->info("Finished setting up basis with {} elements", basis.size());
    return basis;
}

#endif
