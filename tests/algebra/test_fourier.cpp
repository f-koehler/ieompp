#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra/fourier.hpp"
#include "hubbard/algebra/operator.hpp"
#include "hubbard/models/model2d.hpp"

using namespace hubbard;

TEST_CASE("transform", "[algebra]")
{
    using Discretization = model2d::Discretization<double>;
    using Operator = algebra::Operator<Discretization::IndexType, bool>;
    using Transformation =
        algebra::Transformation<algebra::Term<Operator, Complex>, algebra::Term<Operator, Complex>,
                                Discretization, Discretization>;

    Discretization real_space(5, 5, 1., 1.);
    Discretization momentum_space(5, 5);

    Transformation transform(real_space, momentum_space);

    SECTION("creator") {
        Transformation::TermAType term = {Complex{1., 0.},
                                          {Operator{true, Discretization::IndexType{0, 0}, true}}};
        algebra::TermList<Transformation::TermBType> result;
        transform.transform(term, result);
    }
}
