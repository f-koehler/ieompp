#include "operator.hpp"
#include <ieompp/algebra/operator/make.hpp>
using namespace ieompp::algebra;

// TODO: the next three tests are very basic
TEST_CASE("make_creator_types_1")
{
    auto op1       = make_creator(1ul);
    std::size_t ul = 1ul;
    auto op2       = make_creator(ul);

    static_assert(std::is_same<decltype(op1), decltype(op2)>::value,
                  "op1 and op2 should be of same type");
}

TEST_CASE("make_creator_types_2")
{
    auto op1       = make_creator(1ul, true);
    std::size_t ul = 1ul;
    bool b         = false;
    auto op2       = make_creator(ul, b);

    static_assert(std::is_same<decltype(op1), decltype(op2)>::value,
                  "op1 and op2 should be of same type");
}

TEST_CASE("make_creator_types_3")
{
    auto op1       = make_creator(1ul, true, 'c');
    std::size_t ul = 1ul;
    bool b         = false;
    const char c   = 'a';
    auto op2       = make_creator(ul, b, c);

    static_assert(std::is_same<decltype(op1), decltype(op2)>::value,
                  "op1 and op2 should be of same type");
}

TEST_CASE("make_creator_1")
{
    Operator1 op = make_creator(1ul);

    REQUIRE(op.creator);
    REQUIRE(op.index == 1ul);
}

TEST_CASE("make_creator_2")
{
    Operator2 op = make_creator(1ul, true);

    REQUIRE(op.creator);
    REQUIRE(op.index1 == 1ul);
    REQUIRE(op.index2 == true);
}

TEST_CASE("make_creator_3")
{
    Operator3 op = make_creator(1ul, true, 'a');

    REQUIRE(op.creator);
    REQUIRE(std::get<0>(op.indices) == 1ul);
    REQUIRE(std::get<1>(op.indices) == true);
    REQUIRE(std::get<2>(op.indices) == 'a');
}

TEST_CASE("make_annihilator_1")
{
    Operator1 op = make_annihilator(1ul);

    REQUIRE(!op.creator);
    REQUIRE(op.index == 1ul);
}

TEST_CASE("make_annihilator_2")
{
    Operator2 op = make_annihilator(1ul, true);

    REQUIRE(!op.creator);
    REQUIRE(op.index1 == 1ul);
    REQUIRE(op.index2 == true);
}

TEST_CASE("make_annihilator_3")
{
    Operator3 op = make_annihilator(1ul, true, 'a');

    REQUIRE(!op.creator);
    REQUIRE(std::get<0>(op.indices) == 1ul);
    REQUIRE(std::get<1>(op.indices) == true);
    REQUIRE(std::get<2>(op.indices) == 'a');
}
