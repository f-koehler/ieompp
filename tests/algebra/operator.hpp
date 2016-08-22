#ifndef TEST_OPERATOR_HPP_
#define TEST_OPERATOR_HPP_

#include <type_traits>

#include <boost/test/unit_test.hpp>

#include <ieompp/algebra/operator/io.hpp>
#include <ieompp/algebra/operator/operator.hpp>
using namespace ieompp::algebra;

using Op1 = Operator<std::size_t>;
using Op2 = Operator<std::size_t, bool>;
using Op3 = Operator<std::size_t, bool, char>;

#endif
