#include <iostream>
using namespace std;

#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    using Discretization = discretization::LinearDiscretization<double>;
    using Index          = Discretization::Index;
    using Operator       = algebra::Operator<Index, bool>;
    using Term           = algebra::Term<Operator, std::complex<double>>;
    using TermList       = algebra::TermList<Term>;

    Discretization disc(5, 1.);

    /* Term initial = algebra::make_term(Complex{1., 0.}, {algebra::make_creator(0ul, true)}); */
    /* TermList first_commutation; */

    /* // commutate with interaction term */
    /* for(const auto& index : disc.indices) { */
    /*     auto term = algebra::make_term(Complex{1., 0.}, {algebra::make_creator(index, true), */
    /*                                                      algebra::make_annihilator(index, true), */
    /*                                                      algebra::make_creator(index, false), */
    /*                                                      algebra::make_annihilator(index, false)}); */
    /*     algebra::commutate(term, initial, first_commutation); */
    /* } */

    /* cout << first_commutation.size() << endl; */
}
