#ifndef TEST_OPERATOR_HPP_
#define TEST_OPERATOR_HPP_

#include "../test.hpp"

#include <type_traits>

#include <ieompp/algebra/operator/io.hpp>
#include <ieompp/algebra/operator/operator.hpp>

using Op1 = ieompp::algebra::Operator<std::size_t>;
using Op2 = ieompp::algebra::Operator<std::size_t, bool>;
using Op3 = ieompp::algebra::Operator<std::size_t, bool, char>;

#endif
