#include <iostream>
#include <regex>
using namespace std;

#include "hubbard/algebra.hpp"
#include "hubbard/abstract.hpp"
using namespace hubbard;
using namespace hubbard::algebra;

int main() {
    auto term =
        make_term(abstract::AbstractPrefactor{Complex(1., 0.), {}},
                  {abstract::make_annihilator("k_1", "s_1"), abstract::make_creator("k_2", "s_2"),
                   abstract::make_annihilator("k_3", "s_3")});
    abstract::AbstractTermList list;
    list.push_back(term);
    algebra::order_term(list.front(), list);
    cout << term << endl;
    for(auto& t : list)
        cout << t << endl;
}
