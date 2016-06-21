#include <array>
#include <iostream>
using namespace std;

#include "ieompp/algebra/commutator.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/algebra/term_checks.hpp"
#include "ieompp/discretization/square.hpp"
namespace algebra        = ieompp::algebra;
namespace discretization = ieompp::discretization;

using Operator = algebra::Operator<int, bool>;
using Term     = algebra::Term<double, Operator>;

bool check_term(const Term& t)
{
    return (algebra::total_spin<1>(t) == 1) && (algebra::total_creations(t) == 1);
}

void commutate_H_kin(const Term& term)
{
    discretization::SquareDiscretization<double, int> lattice(10, 10, 1., 1.);
    const std::array<bool, 2> spins{{false, true}};
    std::vector<typename std::decay<decltype(term)>::type> comm;

    auto kinetic_term = algebra::make_term(
        -1., {algebra::make_creator(0, false), algebra::make_annihilator(0, false)});
    for(auto s : spins) {
        kinetic_term.operators[0].index2 = s;
        kinetic_term.operators[1].index2 = s;
        for(int i : lattice) {
            kinetic_term.operators[0].index1 = i;
            for(auto& vec : lattice.lattice_vectors()) {
                auto j                           = lattice(lattice[i] - vec);
                kinetic_term.operators[1].index1 = j;
                algebra::commutate(kinetic_term, term, comm);
                j                                = lattice(lattice[i] + vec);
                kinetic_term.operators[1].index1 = j;
                algebra::commutate(kinetic_term, term, comm);
            }
        }
    }

    cout << "(1/J)[H_kin, " << term << "]:" << endl;
    for(const auto& t : comm) {
        cout << "\t" << t << endl;
        assert(check_term(t));
    }
    cout << endl;
}

void commutate_H_int(const Term& term)
{
    discretization::SquareDiscretization<double, int> lattice(10, 10, 1., 1.);
    std::vector<Term> comm;

    auto interaction_term1 = algebra::make_term(
        1., {algebra::make_creator(0, true), algebra::make_annihilator(0, true),
             algebra::make_creator(0, false), algebra::make_annihilator(0, false)});
    auto interaction_term2 = algebra::make_term(
        -.5, {algebra::make_creator(0, true), algebra::make_annihilator(0, true)});
    auto interaction_term3 = algebra::make_term(
        -.5, {algebra::make_creator(0, false), algebra::make_annihilator(0, false)});
    for(int i : lattice) {
        interaction_term1.operators[0].index1 = i;
        interaction_term1.operators[1].index1 = i;
        interaction_term1.operators[2].index1 = i;
        interaction_term1.operators[3].index1 = i;
        interaction_term2.operators[0].index1 = i;
        interaction_term2.operators[1].index1 = i;
        interaction_term3.operators[0].index1 = i;
        interaction_term3.operators[1].index1 = i;

        algebra::commutate(interaction_term1, term, comm);
        algebra::commutate(interaction_term2, term, comm);
        algebra::commutate(interaction_term3, term, comm);
    }

    cout << "(1/U)[H_int, " << term << "]:" << endl;
    for(const auto& t : comm) {
        cout << "\t" << t << endl;
        assert(check_term(t));
    }
    cout << endl;
}


int main()
{
    const auto term1 = algebra::make_term(1., {algebra::make_creator(0, true)});
    const auto term2 =
        algebra::make_term(1., {algebra::make_creator(0, true), algebra::make_creator(3, false),
                                algebra::make_annihilator(6, false)});

    commutate_H_kin(term1);
    commutate_H_int(term1);
    commutate_H_kin(term2);
    commutate_H_int(term2);
}
