#include <iostream>
#include <vector>
using namespace std;

#include "ieompp/commutator.hpp"
#include "ieompp/operator.hpp"
#include "ieompp/term.hpp"
#include "ieompp/symbolic/index.hpp"
#include "ieompp/symbolic/prefactor.hpp"

using Index     = ieompp::symbolic::SpaceIndex;
using Spin      = ieompp::symbolic::SpinIndex;
using Prefactor = ieompp::symbolic::Prefactor<double>;

template <typename Term>
void commutate_H_kin(const Term& term)
{
    static const auto kinetic_term1 =
        ieompp::make_term(ieompp::symbolic::make_prefactor(1.),
                          {ieompp::make_creator(Index{"r_1"}, Spin{u8"σ_1"}),
                           ieompp::make_annihilator(Index{"r_2"}, Spin{u8"σ_1"})});
    static const auto kinetic_term2 = ieompp::make_term(
        ieompp::symbolic::make_prefactor(1.),
        {ieompp::make_creator(Index(2), Spin(1)), ieompp::make_annihilator(Index(1), Spin(1))});

    std::vector<ieompp::Term<Prefactor, ieompp::Operator<Index, Spin>>> comm;
    ieompp::commutate(kinetic_term1, term, comm);
    ieompp::commutate(kinetic_term2, term, comm);

    cout << "(1/J)[H_kin, " << term << "] (without sums):" << endl;
    for(const auto& t : comm) cout << "\t" << t << endl;
    cout << endl;
}

template <typename Term>
void commutate_H_int(const Term& term)
{
    static const auto interaction_term1 = ieompp::make_term(
        ieompp::symbolic::make_prefactor(1.), {ieompp::make_creator(Index(1), Spin{u8"↑"}),
                                               ieompp::make_annihilator(Index(1), Spin{u8"↑"}),
                                               ieompp::make_creator(Index(1), Spin{u8"↓"}),
                                               ieompp::make_annihilator(Index(1), Spin{u8"↓"})});
    static const auto interaction_term2 = ieompp::make_term(
        ieompp::symbolic::make_prefactor(-.5), {ieompp::make_creator(Index(1), Spin{u8"↑"}),
                                                ieompp::make_annihilator(Index(1), Spin{u8"↑"})});
    static const auto interaction_term3 = ieompp::make_term(
        ieompp::symbolic::make_prefactor(-.5), {ieompp::make_creator(Index(1), Spin{u8"↓"}),
                                                ieompp::make_annihilator(Index(1), Spin{u8"↓"})});

    std::vector<ieompp::Term<Prefactor, ieompp::Operator<Index, Spin>>> comm;
    ieompp::commutate(interaction_term1, term, comm);
    ieompp::commutate(interaction_term2, term, comm);
    ieompp::commutate(interaction_term3, term, comm);

    cout << "(1/U)[H_int, " << term << "] (without sum):" << endl;
    for(const auto& t : comm) cout << "\t" << t << endl;
    cout << endl;
}

int main()
{
    auto term1 = ieompp::make_term(ieompp::symbolic::make_prefactor(1.),
                                   {ieompp::make_creator(Index{"x_1"}, Spin{u8"↑"})});
    auto term2 = ieompp::make_term(ieompp::symbolic::make_prefactor(1.),
                                   {ieompp::make_creator(Index{"x_1"}, Spin{u8"↑"}),
                                    ieompp::make_creator(Index{"x_1"}, Spin{u8"↓"}),
                                    ieompp::make_annihilator(Index{"x_1}"}, Spin{u8"↓"})});

    commutate_H_kin(term1);
    commutate_H_int(term1);
    commutate_H_kin(term2);
    commutate_H_int(term2);
}
