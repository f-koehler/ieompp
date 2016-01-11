#include <iostream>

#include "hubbard/config.hpp"
#include "hubbard/abstract/parse.hpp"

using namespace std;
using namespace hubbard::abstract::parse;

int main() {
    cout << "version:      \t" << hubbard::version << endl;
    cout << "re_commutator:\t" << components::str_commutator << endl;
    cout << "re_operator:  \t" << components::str_operator << endl;
    cout << "re_kronecker: \t" << components::str_kronecker << endl;
    cout << "re_number:    \t" << components::str_number << endl;
}
