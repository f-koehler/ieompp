#include <iostream>

#include "ieompp/config.hpp"
#include "ieompp/abstract/parse.hpp"

using namespace std;
using namespace ieompp::abstract::parse;

int main() {
    cout << "version:      \t" << ieompp::version << endl;
    cout << "re_commutator:\t" << components::str_commutator << endl;
    cout << "re_operator:  \t" << components::str_operator << endl;
    cout << "re_kronecker: \t" << components::str_kronecker << endl;
    cout << "re_number:    \t" << components::str_number << endl;
}
