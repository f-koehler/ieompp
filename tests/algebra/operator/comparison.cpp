#include "operator.hpp"
using namespace ieompp::algebra;

TEST_CASE("equality_1")
{
    Operator1 a{true, 0ul}, b{true, 1ul}, c{false, 0ul};

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
    Operator2 a{true, 0ul, true}, b{true, 1ul, false}, c{true, 0ul, false}, d{false, 0ul, true};

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
    Operator3 a{true, {0ul, true, 'a'}}, b{true, {1ul, true, 'a'}}, c{true, {0ul, false, 'a'}},
        d{true, {0ul, true, 'b'}}, e{false, {0ul, true, 'a'}};

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
    Operator1 a{true, 0ul}, b{true, 1ul}, c{false, 0ul};

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
    Operator2 a{true, 0ul, true}, b{true, 1ul, false}, c{true, 0ul, false}, d{false, 0ul, true};

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
    Operator3 a{true, {0ul, true, 'a'}}, b{true, {1ul, true, 'a'}}, c{true, {0ul, false, 'a'}},
        d{true, {0ul, true, 'b'}}, e{false, {0ul, true, 'a'}};

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
