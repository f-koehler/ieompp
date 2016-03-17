#include <iostream>
using namespace std;

#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra/hamiltonian_fourier.hpp"
#include "hubbard/algebra/agenda.hpp"
using namespace hubbard;

#include "Eigen/Dense"
#include "quicli.hpp"
using namespace quicli;

int main()
{
    auto term = algebra::make_term(std::complex<double>(1., 0.), {algebra::make_annihilator(0ul, true)});
    discretization::LinearDiscretization<double> discretization(100, 1.);
    algebra::Hamiltonian<decltype(term)> hamiltonian;

    algebra::Agenda<decltype(term)> agenda;
    agenda.commutate(term, 2, hamiltonian, discretization);

    Eigen::MatrixXcd mat = Eigen::MatrixXcd::Zero(agenda.terms().size(), agenda.terms().size());
    std::size_t i = 0;
    for(auto& line : agenda.results()) {
        for(auto& entry : line) {
            /* if(entry.index >= agenda.terms().size()) { */
            /*     cerr << entry.index << endl; */
            /*     std::exit(1); */
            /* } */
            mat(i, entry.index) = entry.prefactor;
        }
        ++i;
    }
    cout << mat << endl;
}
