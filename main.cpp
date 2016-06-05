#include <complex>
#include <iostream>
#include <typeinfo>
using namespace std;

#include "ieompp/anticommutator.hpp"
#include "ieompp/symbolic/index.hpp"
#include "ieompp/hubbard.hpp"

int main()
{
    using Op = ieompp::Operator<int>;
    Op op1{true, 0};
    auto op2 = ieompp::make_creator(0);
    cout << (op1 == op2) << endl;
}
