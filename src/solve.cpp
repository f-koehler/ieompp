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
    algebra::Agenda<decltype(term)> agenda;

    hamiltonian.J = 0.3;

    agenda.commutate(term, 4, hamiltonian, real_space);
    ode::DenseRK4<decltype(agenda)> rk4(agenda);
    Eigen::Matrix<std::complex<double>, Eigen::Dynamic, 1> v = Eigen::Matrix<std::complex<double>, Eigen::Dynamic, 1>::Zero(agenda.terms().size());
    v(0) = std::complex<double>(1.);


    cout << agenda.terms().size() << endl;
    for(auto i = 0; i < 1024; ++i) {
        rk4.step(v, 0.01);
        cout << "step " << i << " completed" << endl;
    }

    /* for(auto& term : agenda.terms()) cout << term << endl; */
}
