#ifndef FERMI_JUMP_HPP_
#define FERMI_JUMP_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/fermi_jump.hpp>
#include <ieompp/models/hubbard/dispersion.hpp>

template <typename Basis, typename Lattice, typename ExpectationValueFunction, typename Float>
auto init_fermi_jump(const Basis& basis, const Lattice& lattice, const ExpectationValueFunction& ev,
                     const Float& filling_factor)
{
    get_loggers().main->info(u8"Init <Δn_{{k_F,↑}}> observable for half-filled model");
    auto jump = ieompp::models::hubbard_real_space::FermiJump1D<double, Basis>(
        basis, lattice,
        [&ev](const typename Basis::Monomial::Operator& a,
              const typename Basis::Monomial::Operator& b) { return ev(a.index1, b.index1); },
        ieompp::models::hubbard::calculate_fermi_momentum_1d(filling_factor));
    get_loggers().main->info(u8"Finished initializing <Δn_{{k_F,↑}}> observable");
    return jump;
}

#endif
