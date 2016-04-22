#include <iostream>
using namespace std;

#include "ieompp/discretization/linear.hpp"
#include "ieompp/hubbard/hamiltonian.hpp"
#include "ieompp/ieom/agenda.hpp"
#include "ieompp/ode/dense.hpp"
#include <fstream>
#include <cassert>
using namespace ieompp;

int main()
{
    auto term =
        algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
    discretization::LinearDiscretization<double> real_space(10, 1.);
    hubbard::Hamiltonian<decltype(term)> hamiltonian;

    auto result     = hamiltonian.commutate(term, real_space);
    std::size_t num = result.size();
    for(std::size_t i = 0; i < num; ++i) {
        hamiltonian.commutate(result[i], real_space, result);
    }

    auto copy    = decltype(result)(result);
    auto ordered = algebra::order_operators(copy);

    cout << "original:" << endl;
    for(auto& t : result) {
        cout << t << endl;
    }

    cout << endl << endl << "ordered:" << endl;
    for(auto& t : ordered) {
        cout << t << endl;
    }
}
