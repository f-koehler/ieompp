#include <iostream>
using namespace std;

#include "hubbard/algebra.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    auto env = environment();
    for(auto& entry : env) {
        std::cout << entry.first << "\t" << entry.second << std::endl;
    }
}
