#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("equality_1")
{
    const auto a = Operator1::make_creator(0ul), b = Operator1::make_creator(1ul),
               c = Operator1::make_annihilator(0ul);

    REQUIRE(a == a);
    REQUIRE(b == b);
    REQUIRE(c == c);

    REQUIRE(!(a == b));
    REQUIRE(!(a == c));

    REQUIRE(!(b == a));
    REQUIRE(!(b == c));

    REQUIRE(!(c == a));
    REQUIRE(!(c == b));
}

TEST_CASE("equality_2")
{
    const auto a = Operator2::make_creator(0ul, true), b = Operator2::make_creator(1ul, false),
               c = Operator2::make_creator(0ul, false), d = Operator2::make_annihilator(0ul, true);

    REQUIRE(a == a);
    REQUIRE(b == b);
    REQUIRE(c == c);
    REQUIRE(d == d);

    REQUIRE(!(a == b));
    REQUIRE(!(a == c));
    REQUIRE(!(a == d));

    REQUIRE(!(b == a));
    REQUIRE(!(b == c));
    REQUIRE(!(b == d));

    REQUIRE(!(c == a));
    REQUIRE(!(c == b));
    REQUIRE(!(c == d));

    REQUIRE(!(d == a));
    REQUIRE(!(d == b));
    REQUIRE(!(d == c));
}

TEST_CASE("equality_3")
{
    const auto a = Operator3::make_creator(0ul, true, 'a'),
               b = Operator3::make_creator(1ul, true, 'a'),
               c = Operator3::make_creator(0ul, false, 'a'),
               d = Operator3::make_creator(0ul, true, 'b'),
               e = Operator3::make_annihilator(0ul, true, 'a');

    REQUIRE(a == a);
    REQUIRE(b == b);
    REQUIRE(c == c);
    REQUIRE(d == d);
    REQUIRE(e == e);

    REQUIRE(!(a == b));
    REQUIRE(!(a == c));
    REQUIRE(!(a == d));
    REQUIRE(!(a == e));

    REQUIRE(!(b == a));
    REQUIRE(!(b == c));
    REQUIRE(!(b == d));
    REQUIRE(!(b == e));

    REQUIRE(!(c == a));
    REQUIRE(!(c == b));
    REQUIRE(!(c == d));
    REQUIRE(!(c == e));

    REQUIRE(!(d == a));
    REQUIRE(!(d == b));
    REQUIRE(!(d == c));
    REQUIRE(!(d == e));

    REQUIRE(!(e == a));
    REQUIRE(!(e == b));
    REQUIRE(!(e == c));
    REQUIRE(!(e == d));
}

TEST_CASE("inequality_1")
{
    const auto a = Operator1::make_creator(0ul), b = Operator1::make_creator(1ul),
               c = Operator1::make_annihilator(0ul);

    REQUIRE(!(a != a));
    REQUIRE(!(b != b));
    REQUIRE(!(c != c));

    REQUIRE(a != b);
    REQUIRE(a != c);

    REQUIRE(b != a);
    REQUIRE(b != c);

    REQUIRE(c != a);
    REQUIRE(c != b);
}

TEST_CASE("inequality_2")
{
    const auto a = Operator2::make_creator(0ul, true), b = Operator2::make_creator(1ul, false),
               c = Operator2::make_creator(0ul, false), d = Operator2::make_annihilator(0ul, true);

    REQUIRE(!(a != a));
    REQUIRE(!(b != b));
    REQUIRE(!(c != c));
    REQUIRE(!(d != d));

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);

    REQUIRE(b != a);
    REQUIRE(b != c);
    REQUIRE(b != d);

    REQUIRE(c != a);
    REQUIRE(c != b);
    REQUIRE(c != d);

    REQUIRE(d != a);
    REQUIRE(d != b);
    REQUIRE(d != c);
}

TEST_CASE("inequality_3")
{
    const auto a = Operator3::make_creator(0ul, true, 'a'),
               b = Operator3::make_creator(1ul, true, 'a'),
               c = Operator3::make_creator(0ul, false, 'a'),
               d = Operator3::make_creator(0ul, true, 'b'),
               e = Operator3::make_annihilator(0ul, true, 'a');

    REQUIRE(!(a != a));
    REQUIRE(!(b != b));
    REQUIRE(!(c != c));
    REQUIRE(!(d != d));
    REQUIRE(!(e != e));

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);
    REQUIRE(a != e);

    REQUIRE(b != a);
    REQUIRE(b != c);
    REQUIRE(b != d);
    REQUIRE(b != e);

    REQUIRE(c != a);
    REQUIRE(c != b);
    REQUIRE(c != d);
    REQUIRE(c != e);

    REQUIRE(d != a);
    REQUIRE(d != b);
    REQUIRE(d != c);
    REQUIRE(d != e);

    REQUIRE(e != a);
    REQUIRE(e != b);
    REQUIRE(e != c);
    REQUIRE(e != d);
}
