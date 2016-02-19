#include <iostream>
#include <regex>

#include "hubbard/abstract.hpp"
#include "quicli.hpp"

using namespace std;
using namespace hubbard;
using namespace quicli;

int main(int argc, char** argv)
{
    Interface cli("fermion_cas");
    cli.positionals("COMMUTATOR", "A commutator of two terms \"[term1, term2]\"", 1);

    ValueMap vm;
    cli.parse(argc, argv, vm);

    if(vm.positionals().size() != 1) {
        cerr << cli.help() << endl;
        return 1;
    }

    auto expression = std::string(vm.positionals()[0]);
    auto ic = hubbard::abstract::parse::is_commutator(expression);

    if(!std::get<0>(ic)) {
        cerr << "Currently only commutators are implemented!" << endl;
        return 1;
    }

    auto left       = std::get<1>(ic);
    auto right      = std::get<2>(ic);
    auto left_term  = abstract::parse::parse_term(left);
    auto right_term = abstract::parse::parse_term(right);

    abstract::AbstractTermList list;
    algebra::commutate(left_term, right_term, list);
    cout << "[" << left << ", " << right << "]=";
    for(auto& t : list)
        cout << "+" << t;
    cout << endl;
}
