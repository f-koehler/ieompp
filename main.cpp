#include <iostream>
#include <regex>
using namespace std;

#include "hubbard/algebra.hpp"
#include "hubbard/types.hpp"
using namespace hubbard;
using namespace hubbard::algebra;

int main() {
    using Operator = Operator<int, bool>;
    using Term     = Term<Operator>;
    using TermList = TermList<Term>;

    TermList list;
    list.push_back(make_term(Complex(1., 0.), {make_annihilator(0, false), make_creator(0, false),
                                               make_annihilator(0, false)}));
    order_term(list.front(), list);
    for(auto& t : list) {
        cout << t << endl;
    }
}
