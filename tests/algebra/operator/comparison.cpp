#include "operator.hpp"
using namespace ieompp::algebra;

BOOST_AUTO_TEST_CASE(equality_1)
{
    Operator1 a{true, 0ul}, b{true, 1ul}, c{false, 0ul};

    BOOST_TEST(a == a);
    BOOST_TEST(b == b);
    BOOST_TEST(c == c);

    BOOST_TEST(!(a == b));
    BOOST_TEST(!(a == c));

    BOOST_TEST(!(b == a));
    BOOST_TEST(!(b == c));

    BOOST_TEST(!(c == a));
    BOOST_TEST(!(c == b));
}

BOOST_AUTO_TEST_CASE(equality_2)
{
    Operator2 a{true, 0ul, true}, b{true, 1ul, false}, c{true, 0ul, false}, d{false, 0ul, true};

    BOOST_TEST(a == a);
    BOOST_TEST(b == b);
    BOOST_TEST(c == c);
    BOOST_TEST(d == d);

    BOOST_TEST(!(a == b));
    BOOST_TEST(!(a == c));
    BOOST_TEST(!(a == d));

    BOOST_TEST(!(b == a));
    BOOST_TEST(!(b == c));
    BOOST_TEST(!(b == d));

    BOOST_TEST(!(c == a));
    BOOST_TEST(!(c == b));
    BOOST_TEST(!(c == d));

    BOOST_TEST(!(d == a));
    BOOST_TEST(!(d == b));
    BOOST_TEST(!(d == c));
}

BOOST_AUTO_TEST_CASE(equality_3)
{
    Operator3 a{true, {0ul, true, 'a'}}, b{true, {1ul, true, 'a'}}, c{true, {0ul, false, 'a'}},
        d{true, {0ul, true, 'b'}}, e{false, {0ul, true, 'a'}};

    BOOST_TEST(a == a);
    BOOST_TEST(b == b);
    BOOST_TEST(c == c);
    BOOST_TEST(d == d);
    BOOST_TEST(e == e);

    BOOST_TEST(!(a == b));
    BOOST_TEST(!(a == c));
    BOOST_TEST(!(a == d));
    BOOST_TEST(!(a == e));

    BOOST_TEST(!(b == a));
    BOOST_TEST(!(b == c));
    BOOST_TEST(!(b == d));
    BOOST_TEST(!(b == e));

    BOOST_TEST(!(c == a));
    BOOST_TEST(!(c == b));
    BOOST_TEST(!(c == d));
    BOOST_TEST(!(c == e));

    BOOST_TEST(!(d == a));
    BOOST_TEST(!(d == b));
    BOOST_TEST(!(d == c));
    BOOST_TEST(!(d == e));

    BOOST_TEST(!(e == a));
    BOOST_TEST(!(e == b));
    BOOST_TEST(!(e == c));
    BOOST_TEST(!(e == d));
}

BOOST_AUTO_TEST_CASE(inequality_1)
{
    Operator1 a{true, 0ul}, b{true, 1ul}, c{false, 0ul};

    BOOST_TEST(!(a != a));
    BOOST_TEST(!(b != b));
    BOOST_TEST(!(c != c));

    BOOST_TEST(a != b);
    BOOST_TEST(a != c);

    BOOST_TEST(b != a);
    BOOST_TEST(b != c);

    BOOST_TEST(c != a);
    BOOST_TEST(c != b);
}

BOOST_AUTO_TEST_CASE(inequality_2)
{
    Operator2 a{true, 0ul, true}, b{true, 1ul, false}, c{true, 0ul, false}, d{false, 0ul, true};

    BOOST_TEST(!(a != a));
    BOOST_TEST(!(b != b));
    BOOST_TEST(!(c != c));
    BOOST_TEST(!(d != d));

    BOOST_TEST(a != b);
    BOOST_TEST(a != c);
    BOOST_TEST(a != d);

    BOOST_TEST(b != a);
    BOOST_TEST(b != c);
    BOOST_TEST(b != d);

    BOOST_TEST(c != a);
    BOOST_TEST(c != b);
    BOOST_TEST(c != d);

    BOOST_TEST(d != a);
    BOOST_TEST(d != b);
    BOOST_TEST(d != c);
}

BOOST_AUTO_TEST_CASE(inequality_3)
{
    Operator3 a{true, {0ul, true, 'a'}}, b{true, {1ul, true, 'a'}}, c{true, {0ul, false, 'a'}},
        d{true, {0ul, true, 'b'}}, e{false, {0ul, true, 'a'}};

    BOOST_TEST(!(a != a));
    BOOST_TEST(!(b != b));
    BOOST_TEST(!(c != c));
    BOOST_TEST(!(d != d));
    BOOST_TEST(!(e != e));

    BOOST_TEST(a != b);
    BOOST_TEST(a != c);
    BOOST_TEST(a != d);
    BOOST_TEST(a != e);

    BOOST_TEST(b != a);
    BOOST_TEST(b != c);
    BOOST_TEST(b != d);
    BOOST_TEST(b != e);

    BOOST_TEST(c != a);
    BOOST_TEST(c != b);
    BOOST_TEST(c != d);
    BOOST_TEST(c != e);

    BOOST_TEST(d != a);
    BOOST_TEST(d != b);
    BOOST_TEST(d != c);
    BOOST_TEST(d != e);

    BOOST_TEST(e != a);
    BOOST_TEST(e != b);
    BOOST_TEST(e != c);
    BOOST_TEST(e != d);
}
