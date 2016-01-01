#include <iostream>
#include <regex>
using namespace std;

#include "hubbard/abstract/parse.hpp"

int main() {
    /* cout << hubbard::abstract::parse::components::str_operator << endl; */
    std::regex r(u8"α$");
    if(!std::regex_match(u8"α", r)) {
        cout << "No match!" << endl;
    } else {
        cout << "Match!" << endl;
    }
}
