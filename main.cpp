#include <complex>
#include <iostream>
#include <typeinfo>
using namespace std;

#include "ieompp/anticommutator.hpp"
#include "ieompp/symbolic/index.hpp"

int main()
{
    auto op1    = ieompp::make_annihilator(ieompp::symbolic::SpaceIndex<int>{1}, ieompp::symbolic::SpinIndex<bool>{true}),
         op2    = ieompp::make_creator(ieompp::symbolic::SpaceIndex<int>{2}, ieompp::symbolic::SpinIndex<bool>{true});
    auto result = ieompp::anticommutator(op1, op2);

    cout << typeid(typename ieompp::index_type<0, decltype(op1)>::type).name() << endl;
    cout << typeid(ieompp::symbolic::SpaceIndex<int>{1}).name() << endl;
    cout << ieompp::has_symbolic_index<decltype(op1)>::value << endl;

    for(auto& k : result) cout << k;
    cout << endl;
}
