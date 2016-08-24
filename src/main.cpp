#include <iostream>
using namespace std;

#include <ieompp/types/number.hpp>

int main()
{
    cout << (1.e-308 == 0.) << '\n';
    cout << ieompp::types::is_zero(1.e-308) << '\n';
    cout << std::numeric_limits<double>::min() << '\n';
    return 0;
}
