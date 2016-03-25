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
    algebra::Agenda<decltype(term)> a;

    hamiltonian.J = 1.;
    hamiltonian.U = 1.;

    a.commutate(term, 2, hamiltonian, real_space);

    cout << "Terms:" << endl;
    for(auto& term : a.terms()) cout << term << endl;
    cout << endl << endl;

    cout << "Results:" << endl;
    std::size_t i = 0;
    for(auto& line : a.results()) {
        cout << a.terms()[i] << ':' << endl;
        for(auto& entry : line) {
            auto t = a.terms()[entry.index];
            t.prefactor = entry.prefactor;
            cout << t << endl;;
        }
        ++i;
        cout << endl;
    }
}
