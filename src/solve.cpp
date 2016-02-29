#include <iostream>
using namespace std;

#include "hubbard/models/model2d.hpp"
#include "hubbard/algebra.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    using Discretization = model2d::Discretization<double>;
    using Index          = Discretization::IndexType;
    /* using Vector         = Discretization::VectorType; */
    using Operator       = algebra::Operator<Index, bool>;
    using Term           = algebra::Term<Operator, hubbard::Complex>;
    using TermList       = algebra::TermList<Term>;

    Discretization disc(100, 100, 1., 1.);

    Term initial = algebra::make_term(Complex{1., 0.}, {algebra::make_creator(Index{0, 0}, true)});
    TermList first_commutation;

    // commutate with interaction term
    for(const auto& index : disc.indices) {
        auto term = algebra::make_term(Complex{1., 0.}, {algebra::make_creator(index, true),
                                                         algebra::make_annihilator(index, true),
                                                         algebra::make_creator(index, false),
                                                         algebra::make_annihilator(index, false)});
        algebra::commutate(term, initial, first_commutation);
    }
}
