#include <iostream>
using namespace std;

#include "hubbard/abstract.hpp"

int main() {
    auto a = make_abstract_creator("k_1", "s'");
    auto b = make_abstract_creator("k_2", "s");
    cout << a << endl;
    cout << b << endl;
    auto c = anticommutator<AbstractOperator, AbstractPrefactor>(a, b);
    cout << c << endl;
}
