#ifndef TEST_MODELS_HUBBARD_MOMENTUM_SPACE_HPP_
#define TEST_MODELS_HUBBARD_MOMENTUM_SPACE_HPP_

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>
#include <ieompp/models/hubbard_momentum_space/excited_fermi_sea.hpp>

#include <catch.hpp>

using Monomial = ieompp::algebra::Monomial<ieompp::algebra::Operator<uint64_t, bool>>;

#endif
