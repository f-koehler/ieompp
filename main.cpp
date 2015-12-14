#include "hubbard/algebra.hpp"
#include <iostream>
using namespace std;

int main() {
    Term<Operator<int, bool>> term {
        { 2.0, 4.0 },
        {
            make_creator(0, true),
            make_annihilator(1, false),
            make_creator(2, true),
            make_annihilator(2, true)
        }
    };
    cout << term << endl;
}
