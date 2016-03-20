#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/algebra/fourier.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/discretization/linear.hpp"

#include <iostream>

using namespace ieompp;

TEST_CASE("transform", "[algebra]")
{
    using Discretization = discretization::LinearDiscretization<double>;
    using Operator       = algebra::Operator<Discretization::Index, bool>;
    using Transformation = algebra::Transformation<algebra::Term<Operator, std::complex<double>>,
                                                   algebra::Term<Operator, std::complex<double>>,
                                                   Discretization, Discretization>;

    Discretization real_space(10, 1.);
    Discretization momentum_space(10);

    Transformation transform(real_space, momentum_space);

    SECTION("creator")
    {
        Transformation::Term term = {std::complex<double>{1., 0.}, {Operator{true, 1, true}}};
        algebra::TermList<Transformation::TermFourier> result;
        transform.transform(term, result);

        REQUIRE(result.size() == 10);
        for(std::size_t i = 0; i < 10; ++i) {
            REQUIRE(result[i].operators.size() == 1);
            REQUIRE(result[i].operators[0].creator);
            REQUIRE(result[i].operators[0].index == i);
            REQUIRE(result[i].operators[0].spin);
        }
    }

    SECTION("annihilator")
    {
        Transformation::Term term = {std::complex<double>{1., 0.}, {Operator{false, 1, true}}};
        algebra::TermList<Transformation::TermFourier> result;
        transform.transform(term, result);

        REQUIRE(result.size() == 10);
        for(std::size_t i = 0; i < 10; ++i) {
            REQUIRE(result[i].operators.size() == 1);
            REQUIRE(!result[i].operators[0].creator);
            REQUIRE(result[i].operators[0].index == i);
            REQUIRE(result[i].operators[0].spin);
        }
    }
}
