#include <iostream>

#include "ieompp/config.hpp"
#include "ieompp/abstract/parse.hpp"

using namespace std;
using namespace ieompp::abstract::parse_new;

int main() {
    cout << "version:    \t" << ieompp::version << endl;
    cout << "exponent:   \t" << regexes::str_exponent << endl;
    cout << "index:      \t" << regexes::str_index << endl;
    cout << "spin:       \t" << regexes::str_spin << endl;
    cout << "subscript:  \t" << regexes::str_subscript << endl;
    cout << "creator:    \t" << regexes::str_creator << endl;
    cout << "annihilator:\t" << regexes::str_annihilator << endl;
    cout << "commutator: \t" << regexes::str_commutator << endl;
}
