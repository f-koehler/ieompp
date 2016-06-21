#include <complex>
#include <iostream>
#include <typeinfo>
using namespace std;

#include "ieompp/algebra/anticommutator.hpp"
#include "ieompp/hubbard.hpp"
#include "ieompp/symbolic/index.hpp"

int main()
{
    using Op = ieompp::algebra::Operator<int>;
    Op op1{true, 0};
    auto op2 = ieompp::algebra::make_creator(0);
    cout << (op1 == op2) << endl;
}
