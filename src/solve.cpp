#include <iostream>
using namespace std;

#include "hubbard/discretization.hpp"

#include "quicli.hpp"
using namespace quicli;

int main(int argc, char** argv)
{
    CLI cli("solve");
    ValueMap vm;

    cli.add(Parameter("--nx")).mandatory(true);
    /* cli.add(Parameter("--ny")).mandatory(true); */
    cli.add(Parameter("--commutations")).default_value("2");

    auto args = quicli::convert(argc, argv);
    cli.parse(args, vm);
    cli.validate(vm);

    /* auto disc = */
    /*     hubbard::discretize(as<size_t>(vm.get_value("--nx")), as<size_t>(vm.get_value("--ny"))); */

    /* cout << cli.get<Parameter>("--nx").mandatory() << endl; */
    auto v = as<std::list<int>>(vm.get_value("--nx"));
    for(auto& val : v) cout << val << endl;
}
