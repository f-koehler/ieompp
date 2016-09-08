#include "operator.hpp"
#include <ieompp/algebra/operator/is_symbolic.hpp>

// check number of indices
static_assert(Op1::number_of_indices == 1, "Op1 should have 1 index");
static_assert(Op2::number_of_indices == 2, "Op2 should have 2 indices");
static_assert(Op3::number_of_indices == 3, "Op3 should have 3 indices");


// check index tuple type
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


// check is_operator
static_assert(is_operator<Op1>::value, "Op1 should be a Operator type");
static_assert(is_operator<Op2>::value, "Op2 should be a Operator type");
static_assert(is_operator<Op3>::value, "Op3 should be a Operator type");
static_assert(!is_operator<int>::value, "int should not be a Operator type");

// check is_symbolic
static_assert(!is_symbolic_operator<Operator<int>>::value, "Operator<int> should not be symbolic");
static_assert(!is_symbolic_operator<Operator<int, bool>>::value,
              "Operator<int, bool> should not be symbolic");
static_assert(is_symbolic_operator<Operator<std::string>>::value,
              "Operator<std::string> should be symbolic");
static_assert(is_symbolic_operator<Operator<int, bool, std::string>>::value,
              "Operator<int, bool, std::string> should be symbolic");

// check is_symbolic index
static_assert(!is_symbolic_index<0, Operator<int>>::value,
              "Index 0 of Operator<int> should not be symbolic");
static_assert(!is_symbolic_index<0, Operator<int, bool>>::value,
              "Index 0 of Operator<int, bool> should not be symbolic");
static_assert(!is_symbolic_index<1, Operator<int, bool>>::value,
              "Index 1 of Operator<int, bool> should not be symbolic");
static_assert(is_symbolic_index<0, Operator<std::string>>::value,
              "Index 0 of Operator<std::string> should be symbolic");
static_assert(!is_symbolic_index<0, Operator<int, bool, std::string>>::value,
              "Index 0 of Operator<int, bool, std::string> should not be symbolic");
static_assert(!is_symbolic_index<1, Operator<int, bool, std::string>>::value,
              "Index 1 of Operator<int, bool, std::string> should not be symbolic");
static_assert(is_symbolic_index<2, Operator<int, bool, std::string>>::value,
              "Index 2 of Operator<int, bool, std::string> should be symbolic");


BOOST_AUTO_TEST_CASE(dummy_operator)
{
}
