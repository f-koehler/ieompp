#include <iostream>
#include <regex>

#include "hubbard/abstract.hpp"

using namespace std;

int main(int argc, char** argv) {
    if(argc != 2) {
        cerr << "usage: cas [term1, term2]" << endl;
        return 1;
    }

    auto expression = std::string(argv[1]);
    auto ic = hubbard::abstract::parse::is_commutator(expression);

    if(!std::get<0>(ic)) {
        cerr << "Currently only commutators are implemented!" << endl;
        return 1;
    }

    auto left       = std::get<1>(ic);
    auto right      = std::get<2>(ic);
    auto left_term  = hubbard::abstract::parse::parse_term(left);
    auto right_term = hubbard::abstract::parse::parse_term(right);

    cout << "left term: " << left_term << endl;
    cout << "right term: " << right_term << endl;
}
