#include <iostream>
using namespace std;

#include "ieompp/abstract.hpp"
#include "ieompp/hubbard/hamiltonian.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/discretization/linear.hpp"
using namespace ieompp;

#include "quicli.hpp"

int main(int argc, char** argv)
{
    quicli::Interface cli("hamiltonian");
    cli.positionals("term", "the term to commutate with the hamiltonian", 1);
    cli.add(quicli::Flag("--interaction-only"));
    cli.add(quicli::Flag("--hopping-only"));

    if(argc == 1) {
        cerr << "Error" << endl;
        exit(1);
    }

    quicli::ValueMap vm;
    cli.parse(argc, argv, vm);

    auto expression    = string(vm.positionals()[0]);
    auto abstract_term = abstract::parse::parse_term(expression);
    vector<algebra::Operator<std::size_t, bool>> operators;
    for(auto& op : abstract_term.operators) {
        bool spin         = (op.spin == u8"↑") || (op.spin == "1");
        std::size_t index = std::stoul(op.index);
        operators.push_back(op.creator ? algebra::make_creator(index, spin)
                                       : algebra::make_annihilator(index, spin));
    }

    auto term = algebra::Term<algebra::Operator<std::size_t, bool>, std::complex<double>>{
        abstract_term.prefactor.number, operators};

    discretization::LinearDiscretization<double> lattice(10, 1.);
    hubbard::Hamiltonian<decltype(term)> hamiltonian;


    algebra::TermList<decltype(term)> result;
    if(vm.count("--interaction-only"))
        hamiltonian.commutate_interaction(term, lattice, result);
    else if(vm.count("--hopping-only"))
        hamiltonian.commutate_hopping(term, lattice, result);
    else
        hamiltonian.commutate(term, lattice, result);

    for(auto& t : result)
        cout << t << endl;

    return 0;
}
