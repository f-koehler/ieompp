#include "quicli.hpp"
#include <iostream>
using namespace quicli;

int main(int argc, char** argv)
{
    CLI cli("solve");
    ValueMap vm;

    cli.add(Parameter("--nx")).mandatory(true);
    cli.add(Parameter("--ny")).mandatory(true);
    cli.add(Parameter("--commutations")).default_value("2");

    auto args = quicli::convert(argc, argv);
    cli.parse(args, vm);
    cli.validate(vm);
}
