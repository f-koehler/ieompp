#ifndef BASIS3_HPP_
#define BASIS3_HPP_

#include "../include/logging.hpp"

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>

using Operator = ieompp::algebra::Operator<uint64_t, bool>;
using Monomial = ieompp::algebra::Monomial<Operator>;
using Basis    = ieompp::models::hubbard_real_space::Basis3Operator<Monomial>;

template <typename Lattice>
auto init_basis(const Lattice& lattice)
{
    get_loggers().main->info("Set up basis");
    auto basis = Basis(lattice);
    get_loggers().main->info("Finished setting up basis with {} elements", basis.size());
    return basis;
}

#endif
