#include <iostream>
using namespace std;

#include "hubbard/abstract.hpp"

int main() {
    AbstractTerm a, b;
    a.operators.push_back(make_abstract_creator("k_1", "s'"));
    a.operators.push_back(make_abstract_annihilator("k_1", "s'"));
    a.prefactor.prefactor = Complex(1., 0.);
    
    b.operators.push_back(make_abstract_creator("q", "s"));
    b.prefactor.prefactor = Complex(1., 0.);

    auto c = commutate(a, b);
    for(auto& term : c)
        cout << term << endl;
}
