#include <iostream>
#include <vector>
using namespace std;

#include "ieompp/algebra/commutator.hpp"
#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/symbolic/index.hpp"
#include "ieompp/symbolic/prefactor.hpp"
namespace algebra  = ieompp::algebra;
namespace symbolic = ieompp::symbolic;

using Index     = symbolic::SpaceIndex;
using Spin      = symbolic::SpinIndex;
using Prefactor = symbolic::Prefactor<double>;

template <typename Term>
void commutate_H_kin(const Term& term)
{
    static const auto kinetic_term1 = algebra::make_term(
        symbolic::make_prefactor(1.), {algebra::make_creator(Index{"r_1"}, Spin{u8"σ_1"}),
                                       algebra::make_annihilator(Index{"r_2"}, Spin{u8"σ_1"})});
    static const auto kinetic_term2 = algebra::make_term(
        symbolic::make_prefactor(1.),
        {algebra::make_creator(Index(2), Spin(1)), algebra::make_annihilator(Index(1), Spin(1))});

    std::vector<algebra::Term<Prefactor, algebra::Operator<Index, Spin>>> comm;
    algebra::commutate(kinetic_term1, term, comm);
    algebra::commutate(kinetic_term2, term, comm);

    cout << "(1/J)[H_kin, " << term << "] (without sums):" << endl;
    for(const auto& t : comm) cout << "\t" << t << endl;
    cout << endl;
}

template <typename Term>
void commutate_H_int(const Term& term)
{
    static const auto interaction_term1 = algebra::make_term(
        symbolic::make_prefactor(1.), {algebra::make_creator(Index(1), Spin{u8"↑"}),
                                       algebra::make_annihilator(Index(1), Spin{u8"↑"}),
                                       algebra::make_creator(Index(1), Spin{u8"↓"}),
                                       algebra::make_annihilator(Index(1), Spin{u8"↓"})});
    static const auto interaction_term2 = algebra::make_term(
        symbolic::make_prefactor(-.5), {algebra::make_creator(Index(1), Spin{u8"↑"}),
                                        algebra::make_annihilator(Index(1), Spin{u8"↑"})});
    static const auto interaction_term3 = algebra::make_term(
        symbolic::make_prefactor(-.5), {algebra::make_creator(Index(1), Spin{u8"↓"}),
                                        algebra::make_annihilator(Index(1), Spin{u8"↓"})});

    std::vector<algebra::Term<Prefactor, algebra::Operator<Index, Spin>>> comm;
    algebra::commutate(interaction_term1, term, comm);
    algebra::commutate(interaction_term2, term, comm);
    algebra::commutate(interaction_term3, term, comm);

    cout << "(1/U)[H_int, " << term << "] (without sum):" << endl;
    for(const auto& t : comm) cout << "\t" << t << endl;
    cout << endl;
}

int main()
{
    auto term1 = algebra::make_term(symbolic::make_prefactor(1.),
                                    {algebra::make_creator(Index{"x_1"}, Spin{u8"↑"})});
    auto term2 = algebra::make_term(symbolic::make_prefactor(1.),
                                    {algebra::make_creator(Index{"x_1"}, Spin{u8"↑"}),
                                     algebra::make_creator(Index{"x_1"}, Spin{u8"↓"}),
                                     algebra::make_annihilator(Index{"x_1"}, Spin{u8"↓"})});

    commutate_H_kin(term1);
    commutate_H_int(term1);
    commutate_H_kin(term2);
    commutate_H_int(term2);
}
