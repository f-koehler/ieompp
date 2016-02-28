#include <iostream>
using namespace std;

#include "hubbard/algebra.hpp"
#include "hubbard/square2d.hpp"
using namespace hubbard;
using namespace hubbard::square2d;

#include "quicli.hpp"
using namespace quicli;

int main(int argc, char** argv)
{
    auto env = environment();
    for(auto& entry : env) {
        std::cout << entry.first << "\t" << entry.second << std::endl;
    }
}
