#include <iostream>
using namespace std;

#include "ieompp/discretization/linear.hpp"
#include "ieompp/hubbard/hamiltonian.hpp"
#include "ieompp/algebra/agenda.hpp"
#include "ieompp/ode/dense.hpp"
#include <fstream>
using namespace ieompp;

int main()
{
    auto term =
        algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
    discretization::LinearDiscretization<double> real_space(10, 1.);
    hubbard::Hamiltonian<decltype(term)> hamiltonian;
    algebra::Agenda<decltype(hamiltonian)> a;

    hamiltonian.J = 1.;
    hamiltonian.U = 1.;

    a.commutate(term, 1, hamiltonian, real_space);
    cout << a << endl;
}
