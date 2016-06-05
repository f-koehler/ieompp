#include <array>
#include <iostream>
using namespace std;

#include "ieompp/commutator.hpp"
#include "ieompp/operator.hpp"
#include "ieompp/term.hpp"

void commutate_H_kin(const ieompp::Term<double, ieompp::Operator<int, bool>>& term)
{
    const std::array<bool, 2> spins{{false, true}};
    std::vector<typename std::decay<decltype(term)>::type> comm;

    auto kinetic_term =
        ieompp::make_term(-1., {ieompp::make_creator(0, false), ieompp::make_annihilator(0, false)});
    for(auto s : spins) {
        kinetic_term.operators[0].index2 = s;
        kinetic_term.operators[1].index2 = s;
        for(int i = -10; i <= 10; ++i) {
            kinetic_term.operators[0].index1 = i;
            for(int j = i - 1; j <= i + 1; j += 2) {
                kinetic_term.operators[1].index1 = j;
                ieompp::commutate(kinetic_term, term, comm);
            }
        }
    }

    cout << "(1/J)[H_kin, " << term << "]:" << endl;
    for(const auto& t : comm) cout << "\t" << t << endl;
    cout << endl;
}

void commutate_H_int(const ieompp::Term<double, ieompp::Operator<int, bool>>& term)
{
    std::vector<typename std::decay<decltype(term)>::type> comm;

    auto interaction_term1 =
        ieompp::make_term(1., {ieompp::make_creator(0, true), ieompp::make_annihilator(0, true),
                               ieompp::make_creator(0, false), ieompp::make_annihilator(0, false)});
    auto interaction_term2 =
        ieompp::make_term(-.5, {ieompp::make_creator(0, true), ieompp::make_annihilator(0, true)});
    auto interaction_term3 = ieompp::make_term(
        -.5, {ieompp::make_creator(0, false), ieompp::make_annihilator(0, false)});
    for(int i = -10; i <= 10; ++i) {
        interaction_term1.operators[0].index1 = i;
        interaction_term1.operators[1].index1 = i;
        interaction_term1.operators[2].index1 = i;
        interaction_term1.operators[3].index1 = i;
        interaction_term2.operators[0].index1 = i;
        interaction_term2.operators[1].index1 = i;
        interaction_term3.operators[0].index1 = i;
        interaction_term3.operators[1].index1 = i;

        ieompp::commutate(interaction_term1, term, comm);
        ieompp::commutate(interaction_term2, term, comm);
        ieompp::commutate(interaction_term3, term, comm);
    }

    cout << "(1/J)[H_int, " << term << "]:" << endl;
    for(const auto& t : comm) cout << "\t" << t << endl;
    cout << endl;
}


int main()
{
    const auto term1 = ieompp::make_term(1., {ieompp::make_creator(0, true)});
    const auto term2 = ieompp::make_term(1., {ieompp::make_creator(1, true)});
    const auto term3 = ieompp::make_term(1., {ieompp::make_creator(-1, true)});
    const auto term4 =
        ieompp::make_term(1., {ieompp::make_creator(0, true), ieompp::make_creator(0, false),
                               ieompp::make_annihilator(0, false)});

    commutate_H_kin(term1);
    commutate_H_int(term1);
    commutate_H_kin(term2);
    commutate_H_int(term2);
    commutate_H_kin(term3);
    commutate_H_int(term3);
    commutate_H_kin(term4);
    commutate_H_int(term4);
}