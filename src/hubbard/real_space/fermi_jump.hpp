#ifndef FERMI_JUMP_HPP_
#define FERMI_JUMP_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/fermi_jump.hpp>

template <typename Basis, typename Lattice, typename ExpectationValueFunction>
auto init_fermi_jump(const Basis& basis, const Lattice& lattice, const ExpectationValueFunction& ev)
{
    static_cast<void>(basis);
    get_loggers().main->info(u8"Init <Δn_{{k_F,↑}}> observable for half-filled model");
    auto site_occupation = ieompp::models::hubbard_real_space::FermiJump1D<double, Basis>{
        lattice, [&ev](const typename Basis::Monomial::Operator& a,
                       const typename Basis::Monomial::Operator& b) { return ev(a, b); }};
    get_loggers().main->info(u8"Finished initializing <Δn_{{k_F,↑}}> observable");
    return site_occupation;
}

#endif
