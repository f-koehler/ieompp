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

BOOST_AUTO_TEST_CASE(test_same_indices_1)
{
    Op1 a{true, 0ul}, b{false, 1ul};

    BOOST_TEST(a.same_indices(a));
    BOOST_TEST(b.same_indices(b));

    BOOST_TEST(!a.same_indices(b));
    BOOST_TEST(!b.same_indices(a));
}

BOOST_AUTO_TEST_CASE(test_same_indices_2)
{
    Op2 a{true, 0ul, true}, b{false, 0ul, false}, c{true, 1ul, true};

    BOOST_TEST(a.same_indices(a));
    BOOST_TEST(b.same_indices(b));
    BOOST_TEST(c.same_indices(c));

    BOOST_TEST(!a.same_indices(b));
    BOOST_TEST(!a.same_indices(c));

    BOOST_TEST(!b.same_indices(a));
    BOOST_TEST(!b.same_indices(c));

    BOOST_TEST(!c.same_indices(a));
    BOOST_TEST(!c.same_indices(b));
}

BOOST_AUTO_TEST_CASE(test_same_indices_3)
{
    Op3 a{true, {0ul, true, 'a'}}, b{false, {0ul, true, 'b'}}, c{true, {0ul, false, 'a'}},
        d{false, {1ul, true, 'a'}};

    BOOST_TEST(a.same_indices(a));
    BOOST_TEST(b.same_indices(b));
    BOOST_TEST(c.same_indices(c));
    BOOST_TEST(d.same_indices(d));

    BOOST_TEST(!a.same_indices(b));
    BOOST_TEST(!a.same_indices(c));
    BOOST_TEST(!a.same_indices(d));

    BOOST_TEST(!b.same_indices(a));
    BOOST_TEST(!b.same_indices(c));
    BOOST_TEST(!b.same_indices(d));

    BOOST_TEST(!c.same_indices(a));
    BOOST_TEST(!c.same_indices(b));
    BOOST_TEST(!c.same_indices(d));

    BOOST_TEST(!d.same_indices(a));
    BOOST_TEST(!d.same_indices(b));
    BOOST_TEST(!d.same_indices(c));
}

BOOST_AUTO_TEST_CASE(test_index_tuple_1)
{
    Op1 op{true, 1ul};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul)));
}

BOOST_AUTO_TEST_CASE(test_index_tuple_2)
{
    Op2 op{true, 1ul, true};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul, true)));
}

BOOST_AUTO_TEST_CASE(test_index_tuple_3)
{
    Op3 op{true, {1ul, true, 'a'}};

    BOOST_TEST((op.index_tuple() == std::make_tuple(1ul, true, 'a')));
}

BOOST_AUTO_TEST_CASE(make_creator_1)
{
    Op1 op = make_creator(1ul);

    BOOST_TEST(op.creator);
    BOOST_TEST(op.index == 1ul);
}

BOOST_AUTO_TEST_CASE(make_creator_2)
{
    Op2 op = make_creator(1ul, true);

    BOOST_TEST(op.creator);
    BOOST_TEST(op.index1 == 1ul);
    BOOST_TEST(op.index2 == true);
}

BOOST_AUTO_TEST_CASE(make_creator_3)
{
    Op3 op = make_creator(1ul, true, 'a');

    BOOST_TEST(op.creator);
    BOOST_TEST(std::get<0>(op.indices) == 1ul);
    BOOST_TEST(std::get<1>(op.indices) == true);
    BOOST_TEST(std::get<2>(op.indices) == 'a');
}

BOOST_AUTO_TEST_CASE(make_annihilator_1)
{
    Op1 op = make_annihilator(1ul);

    BOOST_TEST(!op.creator);
    BOOST_TEST(op.index == 1ul);
}

BOOST_AUTO_TEST_CASE(make_annihilator_2)
{
    Op2 op = make_annihilator(1ul, true);

    BOOST_TEST(!op.creator);
    BOOST_TEST(op.index1 == 1ul);
    BOOST_TEST(op.index2 == true);
}

BOOST_AUTO_TEST_CASE(make_annihilator_3)
{
    Op3 op = make_annihilator(1ul, true, 'a');

    BOOST_TEST(!op.creator);
    BOOST_TEST(std::get<0>(op.indices) == 1ul);
    BOOST_TEST(std::get<1>(op.indices) == true);
    BOOST_TEST(std::get<2>(op.indices) == 'a');
}
