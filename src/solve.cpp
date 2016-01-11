#include "quicli.hpp"

int main()
{
    quicli::CLI cli;
    auto& v = cli.add(quicli::Value({"--kx"}));
}
