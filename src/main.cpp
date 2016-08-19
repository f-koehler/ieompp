#include <iostream>
#include <vector>
using namespace std;

#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/discretization/linear.hpp"
#include "ieompp/hubbard/explicit_momentum_space.hpp"
#include "ieompp/algorithm.hpp"
using namespace ieompp::algebra;

int main()
{
    std::vector<int> v{2,2};
    std::vector<int> res;
    ieompp::globally_unique_with_merge(v.begin(), v.end(), std::back_inserter(res));
    for(auto& i : res) {
        cout << i << endl;
    }
    /* ieompp::discretization::LinearDiscretization<double, long> momentum_space(20); */
    /* ieompp::discretization::LinearDiscretization<double, long> lattice(20, 1.); */

    /* /1* auto initial = make_term(1., {make_creator(0l, true)}); *1/ */
    /* auto initial = make_term(1., {make_creator(0l, true), make_creator(0l, false), make_annihilator(0l, false)}); */

    /* ieompp::hubbard::momentum_space::Hamiltonian<double> hamiltonian{1., 1.}; */
    /* std::vector<decltype(initial)> result; */
    /* hamiltonian.generate_interaction_terms(initial, momentum_space, lattice, result); */

    /* for(auto& t : result) { */
    /*     auto mom = 0.; */
    /*     for(auto& op : t.operators) { */
    /*         mom += (op.creator ? 1. : -1.) * momentum_space[op.index1]; */
    /*     } */
    /*     cout << mom << "\t" << t << endl; */
    /* } */

    /* return 0; */
}
