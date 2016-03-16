#include <iostream>
using namespace std;

#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra/hamiltonian_fourier.hpp"
#include "hubbard/algebra/agenda.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    auto term = algebra::make_term(std::complex<double>(1., 0.), {algebra::make_annihilator(0ul, true)});
    discretization::LinearDiscretization<double> discretization(100, 1.);
    algebra::Hamiltonian<decltype(term)> hamiltonian;

    algebra::Agenda<decltype(term)> agenda;
    agenda.commutate(term, 4, hamiltonian, discretization);
    cout << agenda.terms().size() << endl;
}
