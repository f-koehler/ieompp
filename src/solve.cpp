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
    discretization::LinearDiscretization<double> real_space(10, 1.);
    algebra::Hamiltonian<decltype(term)> hamiltonian;
    algebra::Agenda<decltype(term)> a1, a2;

    hamiltonian.J = 0.3;

    a1.commutate(term, 4, hamiltonian, real_space);
    a2.commutate(term, 4, hamiltonian, real_space);
    a2.join(a1);

    cout << a1.terms().size() << " " << a2.terms().size() << endl;
}
