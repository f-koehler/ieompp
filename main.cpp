#include <iostream>
using namespace std;

#include "ieompp/operator.hpp"
#include "ieompp/tuple.hpp"

int main()
{
    cout << ieompp::Operator<int, int>::number_of_indices << endl;
}
