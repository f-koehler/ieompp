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
    discretization::LinearDiscretization<double> discretization(100, 1.);
    algebra::Hamiltonian<decltype(term)> hamiltonian;

    algebra::Agenda<decltype(term)> agenda;
    agenda.commutate(term, 3, hamiltonian, discretization);

    Eigen::MatrixXcd mat = Eigen::MatrixXcd::Zero(agenda.terms().size(), agenda.terms().size());
    std::size_t i = 0;
    for(auto& line : agenda.results()) {
        for(auto& entry : line) {
            mat(i, entry.index) = entry.prefactor;
        }
        ++i;
    }
}
