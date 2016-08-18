#include <iostream>
#include <vector>
using namespace std;

#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/discretization/linear.hpp"
#include "ieompp/hubbard/explicit_momentum_space.hpp"
using namespace ieompp::algebra;

int main()
{
    ieompp::discretization::LinearDiscretization<double, long> momentum_space(10);
    ieompp::discretization::LinearDiscretization<double, long> lattice(10, 1.);

    auto initial = make_term(1., {make_creator(0l, true)});
    /* auto initial = make_term(1., {make_creator(0l, true), make_creator(0l, false), make_annihilator(0l, false)}); */

    ieompp::hubbard::momentum_space::Hamiltonian<double> hamiltonian{1., 1.};
    std::vector<decltype(initial)> result;
    hamiltonian.generate_terms(initial, momentum_space, lattice, result);

    cout << momentum_space[initial.operators[0].index1] << endl;
    for(auto& t : result) {
        auto mom = 0.;
        for(auto& op : t.operators) {
            mom += (op.creator ? 1. : -1.) * momentum_space[op.index1];
        }
        cout << mom << endl;
    }

    return 0;
}
