#include "quicli.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    quicli::CLI cli("solve");
    quicli::ValueMap vm;
    cli.add(quicli::Flag({"--help", "-h"}));

    auto args = quicli::convert(argc, argv);
    cli.parse(args, vm);
    cli.validate(vm);

    for(auto& item : vm) {
        std::cout << item.first << "\t" << item.second.front().front() << std::endl;
    }
}
