#ifndef TEST_ALGEBRA_MONOMIAL_HPP_
#define TEST_ALGEBRA_MONOMIAL_HPP_

#include <ieompp/algebra/monomial/io.hpp>
#include <ieompp/algebra/monomial/monomial.hpp>
#include <ieompp/algebra/operator/io.hpp>

#include <type_traits>

#include <boost/test/unit_test.hpp>

using Operator1 = ieompp::algebra::Operator<int>;
using Monomial1 = ieompp::algebra::Monomial<Operator1>;

static_assert(std::is_same<Monomial1::Operator, Operator1>::value,
              "Operator type of Monomial<Operator1> must be Operator1");

#endif
