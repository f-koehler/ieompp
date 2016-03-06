#include <iostream>
using namespace std;

#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra/hamiltonian.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    hubbard::discretization::LinearDiscretization<double> discretization(1000, 1.);
    hubbard::algebra::
        Hamiltonian<hubbard::algebra::Term<hubbard::algebra::Operator<std::size_t, bool>,
                                           std::complex<double>>> hamiltonian;
    auto term = hubbard::algebra::make_term(std::complex<double>(1., 0.),
                                            {hubbard::algebra::make_annihilator(0ul, true)});
    hubbard::algebra::TermList<decltype(term)> result;
    /* hamiltonian.commutate_hopping(term, discretization, result); */
    hamiltonian.commutate_interaction(term, discretization, result);

    for(auto& term : result) {
        cout << term << endl;
    }
}
