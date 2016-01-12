#include <iostream>
using namespace std;

#include "hubbard/ieom/discretization.hpp"

#include "quicli.hpp"
using namespace quicli;

int main(int argc, char** argv)
{
    CLI cli("solve");
    ValueMap vm;

    cli.add(Parameter("--nx")).mandatory(true);
    cli.add(Parameter("--ny")).mandatory(true);
    cli.add(Parameter("--kx")).mandatory(true);
    cli.add(Parameter("--ky")).mandatory(true);
    cli.add(Parameter("--commutations")).default_value("2");

    auto args = quicli::convert(argc, argv);
    cli.parse(args, vm);
    cli.validate(vm);

    auto disc = hubbard::ieom::discretize(as<size_t>(vm.get("--nx")), as<size_t>(vm.get("--ny")));
    auto kx = disc.kx_index(as<double>(vm.get("--kx")));
    auto ky = disc.ky_index(as<double>(vm.get("--ky")));

    cout << kx << ", " << ky << endl;
}
