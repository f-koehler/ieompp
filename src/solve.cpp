#include <iostream>
using namespace std;

#include "ieompp/discretization/linear.hpp"
#include "ieompp/algebra/hamiltonian_fourier.hpp"
#include "ieompp/algebra/agenda.hpp"
using namespace ieompp;

#include "Eigen/Dense"
#include "quicli.hpp"
using namespace quicli;

int main()
{
    auto term = algebra::make_term(std::complex<double>(1., 0.), {algebra::make_annihilator(0ul, true)});
    discretization::LinearDiscretization<double> fourier_space(3), real_space(10, 1.);
    algebra::HamiltonianFourier<decltype(term)> hamiltonian;

    algebra::TermList<decltype(term)> result;

    hamiltonian.commutate_interaction(term, fourier_space, real_space, result);

    for(auto term : result) {
        double k = 0.;
        for(auto& op : term.operators) k += fourier_space[op.index] * (op.creator ? 1. : -1.);
        cout << term << "\t" << k << endl;
    }
}
