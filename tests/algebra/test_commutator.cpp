#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/commutator.hpp"
#include "ieompp/algebra/operator.hpp"

using namespace ieompp;
using namespace algebra;
#include <iostream>

template <typename Operator>
void test_anticommutates()
{
    using Index    = typename Operator::Index;
    using Spin     = typename Operator::Spin;

    SECTION("creator & creator")
    {
        auto ret = anticommutates(make_creator(Index(0), Spin(true)), make_creator(Index(0), Spin(true)));
        REQUIRE(ret == true);

        ret = anticommutates(make_creator(Index(0), Spin(false)), make_creator(Index(0), Spin(true)));
        REQUIRE(ret == true);

        ret = anticommutates(make_creator(Index(1), Spin(true)), make_creator(Index(0), Spin(true)));
        REQUIRE(ret == true);
    }

    SECTION("annihilator & creator")
    {
        auto ret = anticommutates(make_annihilator(Index(0), Spin(true)), make_creator(Index(0), Spin(true)));
        REQUIRE(ret == false);

        ret = anticommutates(make_annihilator(Index(0), Spin(false)), make_creator(Index(0), Spin(true)));
        REQUIRE(ret == true);

        ret = anticommutates(make_annihilator(Index(1), Spin(true)), make_creator(Index(0), Spin(true)));
        REQUIRE(ret == true);
    }

    SECTION("annihilator & annihilator")
    {
        auto ret = anticommutates(make_annihilator(Index(0), Spin(true)), make_annihilator(Index(0), Spin(true)));
        REQUIRE(ret == true);

        ret = anticommutates(make_annihilator(Index(0), Spin(false)), make_annihilator(Index(0), Spin(true)));
        REQUIRE(ret == true);

        ret = anticommutates(make_annihilator(Index(1), Spin(true)), make_annihilator(Index(0), Spin(true)));
        REQUIRE(ret == true);
    }
}

template <typename Operator, typename Real>
void test_commutator()
{
    using Complex  = std::complex<Real>;
    using Index    = typename Operator::Index;
    using Spin     = typename Operator::Spin;
    using Term     = Term<Operator, Complex>;
    using TermList = TermList<Term>;

    SECTION("[cc,c]")
    {
        TermList list;
        auto term1 = make_term(Complex{1., 0.}, {make_creator(Index(0), Spin(true)), make_annihilator(Index(1), Spin(true))});
        auto term2 = make_term(Complex{1., 0.}, {make_creator(Index(1), Spin(true))});

        commutate(term1, term2, list);
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == make_term(Complex{1., 0.}, {make_creator(Index(0), Spin(true))}));
    }

    SECTION("[c,cc]")
    {
        TermList list;
        auto term1 = make_term(Complex{1., 0.}, {make_creator(Index(1), Spin(true))});
        auto term2 = make_term(Complex{1., 0.}, {make_creator(Index(0), Spin(true)), make_annihilator(Index(1), Spin(true))});

        commutate(term1, term2, list);
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == make_term(Complex{-1., 0.}, {make_creator(Index(0), Spin(true))}));
    }

    SECTION("[cc,cc]")
    {
        TermList list;
        auto term1 = make_term(Complex{1., 0.}, {make_creator(Index(0), Spin(true)), make_annihilator(Index(1), Spin(true))});
        auto term2 = make_term(Complex{1., 0.}, {make_creator(Index(1), Spin(true)), make_annihilator(Index(0), Spin(true))});

        commutate(term1, term2, list);
        REQUIRE(list[0] == make_term(Complex{-1., 0.}, {make_creator(Index(1), Spin(true)), make_annihilator(Index(1), Spin(true))}));
        REQUIRE(list[1] == make_term(Complex{1., 0.}, {make_creator(Index(0), Spin(true)), make_annihilator(Index(0), Spin(true))}));
    }
}

template <typename Operator>
void test_order_operators()
{
    using Index = typename Operator::Index;
    using Spin  = typename Operator::Spin;
    using Term  = Term<Operator, std::complex<double>>;

    SECTION(u8"c^†c^†c")
    {
        TermList<Term> terms = {
            {make_term(std::complex<double>(1.),
                       {make_creator(Index(1), Spin(0)), make_creator(Index(0), Spin(0)),
                        make_annihilator(Index(0), Spin(0))})}};
        auto ordered               = order_operators(terms);
        TermList<Term> expectation = {
            {make_term(std::complex<double>(-1.),
                       {make_creator(Index(0), Spin(0)), make_creator(Index(1), Spin(0)),
                        make_annihilator(Index(0), Spin(0))})}};
        REQUIRE(ordered.size() == expectation.size());
        REQUIRE(std::equal(ordered.begin(), ordered.end(), expectation.begin()));
    }

    SECTION(u8"c^†c^†c^†cc")
    {
        TermList<Term> terms = {
            {make_term(std::complex<double>(1.),
                       {make_creator(Index(0), Spin(0)), make_creator(Index(0), Spin(1)),
                        make_annihilator(Index(0), Spin(0)), make_creator(Index(0), Spin(0)),
                        make_annihilator(Index(0), Spin(0))})}};
        auto ordered               = order_operators(terms);
        TermList<Term> expectation = {
            {make_term(std::complex<double>(1.),
                       {make_creator(Index(0), Spin(0)), make_creator(Index(0), Spin(0)),
                        make_creator(Index(0), Spin(1)), make_annihilator(Index(0), Spin(0)),
                        make_annihilator(Index(0), Spin(0))}),
             make_term(std::complex<double>(1.),
                       {make_creator(Index(0), Spin(0)), make_creator(Index(0), Spin(1)),
                        make_annihilator(Index(0), Spin(0))})}};
        REQUIRE(ordered.size() == expectation.size());
        REQUIRE(std::equal(ordered.begin(), ordered.end(), expectation.begin()));
    }
}

TEST_CASE("anticommutates")
{
    test_anticommutates<Operator<unsigned long, bool>>();
    test_anticommutates<Operator<unsigned long, NoSpin>>();
    test_anticommutates<Operator<long, bool>>();
    test_anticommutates<Operator<long, NoSpin>>();
}

TEST_CASE("commutator")
{
    test_commutator<Operator<long, bool>, float>();
    test_commutator<Operator<long, NoSpin>, float>();
    test_commutator<Operator<long, bool>, double>();
    test_commutator<Operator<long, NoSpin>, double>();
    test_commutator<Operator<long, bool>, long double>();
    test_commutator<Operator<long, NoSpin>, long double>();

    test_commutator<Operator<unsigned long, bool>, float>();
    test_commutator<Operator<unsigned long, NoSpin>, float>();
    test_commutator<Operator<unsigned long, bool>, double>();
    test_commutator<Operator<unsigned long, NoSpin>, double>();
    test_commutator<Operator<unsigned long, bool>, long double>();
    test_commutator<Operator<unsigned long, NoSpin>, long double>();
}

TEST_CASE("order_operators")
{
    test_order_operators<Operator<unsigned long, bool>>();
    test_order_operators<Operator<long, bool>>();
}
