#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra/fourier.hpp"
#include "hubbard/algebra/operator.hpp"
#include "hubbard/model1d/discretization.hpp"

#include <iostream>

using namespace hubbard;

TEST_CASE("transform", "[algebra]")
{
    using Discretization = model1d::Discretization<double>;
    using Operator = algebra::Operator<Discretization::IndexType, bool>;
    using Transformation =
        algebra::Transformation<algebra::Term<Operator, Complex>, algebra::Term<Operator, Complex>,
                                Discretization, Discretization>;

    Discretization real_space(4, 1.);
    Discretization momentum_space(4);

    Transformation transform(real_space, momentum_space);

    SECTION("creator") {
        Transformation::TermAType term = {Complex{1., 0.}, {Operator{true, 1, true}}};
        algebra::TermList<Transformation::TermBType> result;
        transform.transform(term, result);

        REQUIRE(result.size() == 4);
        for(std::size_t i = 0; i < 4; ++i) {
            REQUIRE(result[i].operators.size() == 1);
            REQUIRE(result[i].operators[0].creator);
            REQUIRE(result[i].operators[0].index == i);
            REQUIRE(result[i].operators[0].spin);
        }
    }
    
    SECTION("annihilator") {
        Transformation::TermAType term = {Complex{1., 0.}, {Operator{false, 1, true}}};
        algebra::TermList<Transformation::TermBType> result;
        transform.transform(term, result);

        REQUIRE(result.size() == 4);
        for(std::size_t i = 0; i < 4; ++i) {
            REQUIRE(result[i].operators.size() == 1);
            REQUIRE(!result[i].operators[0].creator);
            REQUIRE(result[i].operators[0].index == i);
            REQUIRE(result[i].operators[0].spin);
        }
    }
}
