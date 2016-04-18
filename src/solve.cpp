#include <iostream>
using namespace std;

#include "ieompp/discretization/linear.hpp"
#include "ieompp/hubbard/hamiltonian.hpp"
#include "ieompp/ieom/agenda.hpp"
#include "ieompp/ode/dense.hpp"
#include <fstream>
using namespace ieompp;

int main()
{
    auto term =
        algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
    discretization::LinearDiscretization<double> real_space(10, 1.);
    hubbard::Hamiltonian<decltype(term)> hamiltonian;
    ieom::Agenda<decltype(hamiltonian)> a;

    hamiltonian.J = 1.;
    hamiltonian.U = 1.;
    hamiltonian.enable_interaction = false;

    a.commutate(term, 2, hamiltonian, real_space);

    for(auto& term : a.system().terms) {
        cout << term << endl;
    }
}
