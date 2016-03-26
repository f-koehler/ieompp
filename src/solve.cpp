#include <iostream>
using namespace std;

#include "ieompp/discretization/linear.hpp"
#include "ieompp/algebra/hamiltonian.hpp"
#include "ieompp/algebra/agenda.hpp"
#include "ieompp/ode/dense.hpp"
#include <fstream>
using namespace ieompp;

int main()
{
    auto term =
        algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
    discretization::LinearDiscretization<double, std::size_t> real_space(0, 9, 1.);
    algebra::Hamiltonian<decltype(term)> hamiltonian;
    algebra::Agenda<decltype(term)> a;

    hamiltonian.J = 1.;
    hamiltonian.U = 1.;

    a.commutate(term, 1, hamiltonian, real_space);
    cout << a << endl;
}
