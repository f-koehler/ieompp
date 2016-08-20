#ifndef TEST_OPERATOR_HPP_
#define TEST_OPERATOR_HPP_

#include <boost/test/unit_test.hpp>

#include <ieompp/algebra/operator.hpp>
using namespace ieompp::algebra;

using Op1 = Operator<std::size_t>;
using Op2 = Operator<std::size_t, bool>;
using Op3 = Operator<std::size_t, bool, char>;

// check number_of_indices
static_assert(Op1::number_of_indices == 1, "Operator<std::size_t> should have 1 index");
static_assert(Op2::number_of_indices == 2, "Operator<std::size_t> should have 2 indices");
static_assert(Op3::number_of_indices == 3, "Operator<std::size_t> should have 3 indices");

// change index tuple type
static_assert(std::is_same<Op1::Indices, std::tuple<std::size_t>>::value,
              "Index tuple should be of type std::tuple<std::size_t>");
static_assert(std::is_same<Op2::Indices, std::tuple<std::size_t, bool>>::value,
              "Index tuple should be of type std::tuple<std::size_t, bool>");
static_assert(std::is_same<Op3::Indices, std::tuple<std::size_t, bool, char>>::value,
              "Index tuple should be of type std::tuple<std::size_t, bool>");

// check types of individual indices
static_assert(std::is_same<Op1::Index, std::size_t>::value, "Index type should be std::size_t");

static_assert(std::is_same<Op2::Index1, std::size_t>::value, "Index type should be std::size_t");
static_assert(std::is_same<Op2::Index2, bool>::value, "Index type should be std::size_t");

#endif
