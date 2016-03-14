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
    auto term =
        algebra::make_term(std::complex<double>(1., 0.), {algebra::make_annihilator(0ul, true)});
    discretization::LinearDiscretization<double> discretization(3), lattice(100, 1.);
    algebra::HamiltonianFourier<decltype(term)> hamiltonian;

    algebra::TermList<decltype(term)> result;
    hamiltonian.commutate_interaction(term, discretization, lattice, result);

    for(auto& t : result) {
        cout << t << endl;
    }
    for(auto& site : discretization.sites) {
        cout << site << endl;
    }
}
