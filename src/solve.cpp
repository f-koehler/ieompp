#include <iostream>
using namespace std;

#include "ieompp/discretization/linear.hpp"
#include "ieompp/algebra/hamiltonian.hpp"
#include "ieompp/algebra/agenda.hpp"
#include "ieompp/ode/dense.hpp"
#include "ieompp/io/algebra.hpp"
#include <fstream>
using namespace ieompp;

int main()
{
    auto term = algebra::make_term(std::complex<double>(1., 0.), {algebra::make_annihilator(0ul, true)});
    discretization::LinearDiscretization<double> real_space(10, 1.);
    algebra::Hamiltonian<decltype(term)> hamiltonian;
    algebra::Agenda<decltype(term)> agenda;

    agenda.commutate(term, 4, hamiltonian, real_space);

    ofstream file("test.bin", ios_base::binary);
    io::write_binary(file, agenda);
    file.close();

    ifstream in_file("test.bin", ios_base::binary);
    io::read_binary(in_file, agenda);
    in_file.close();

    for(auto& term : agenda.terms()) cout << term << endl;
    cout << endl;
}
