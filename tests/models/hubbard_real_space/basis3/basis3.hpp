#ifndef TESTS_MODELS_HUBBARD_REAL_SPACE_BASIS3_HPP_
#define TESTS_MODELS_HUBBARD_REAL_SPACE_BASIS3_HPP_

#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>

using Operator = ieompp::algebra::Operator<uint64_t, bool>;
using Monomial = ieompp::algebra::Monomial<Operator>;

#endif
