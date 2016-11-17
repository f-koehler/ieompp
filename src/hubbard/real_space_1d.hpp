#ifndef SRC_HUBBARD_REAL_SPACE_1d_HPP_
#define SRC_HUBBARD_REAL_SPACE_1d_HPP_

#include "real_space.hpp"

#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
#include <ieompp/models/hubbard_real_space/site_occupation.hpp>

using Basis1  = ieompp::models::hubbard_real_space::Basis1Operator<Term1d>;
using Basis3  = ieompp::models::hubbard_real_space::Basis3Operator<Term1d>;
using Lattice = ieompp::discretization::LinearDiscretization<double, uint64_t>;

template <typename Basis>
Basis init_basis(const Lattice& lattice)
{
    get_loggers().main->info("Set up basis");
    auto basis = Basis(lattice);
    get_loggers().main->info("Finished setting up basis with {} elements", basis.size());
    return basis;
}

template <typename Basis>
ieompp::models::hubbard_real_space::SiteOccupation<Basis>
init_site_occupation_observable(const Lattice& lattice)
{
    get_loggers().main->info(u8"Init <n_{0,↑}> observable for half-filled model");
    auto site_occupation = ieompp::models::hubbard_real_space::SiteOccupation<Basis>{
        ieompp::models::hubbard_real_space::ExpectationValue1DHalfFilled<double, Lattice>{lattice}};
    get_loggers().main->info(u8"Finished initializing <n_{0,↑}> observable");
    return site_occupation;
}

#endif
