#include <iostream>
using namespace std;

#include <ieompp/algebra/commutator.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/symbolic/prefactor.hpp>
using namespace ieompp;
using namespace ieompp::algebra;

int main()
{
    auto hopping1 = make_term(symbolic::make_prefactor(1.),
                              {make_creator("r_i", true), make_annihilator(u8"r_i+δ_j", true)});
    auto hopping2 = make_term(symbolic::make_prefactor(1.),
                              {make_creator(u8"r_i+δ_j", true), make_annihilator("r_i", true)});
    auto hopping3 = make_term(symbolic::make_prefactor(1.),
                              {make_creator("r_i", false), make_annihilator(u8"r_i+δ_j", false)});
    auto hopping4 = make_term(symbolic::make_prefactor(1.),
                              {make_creator(u8"r_i+δ_j", false), make_annihilator("r_i", false)});
    auto interaction = make_term(symbolic::make_prefactor(1.),
                                 {make_creator("r_i", true), make_annihilator("r_i", true),
                                  make_creator("r_i", false), make_annihilator("r_i", false)});
    auto t1 = make_term(symbolic::make_prefactor(1.), {make_creator(u8"ρ", true)});
    auto t2 = make_term(symbolic::make_prefactor(1.),
                        {make_creator(u8"ρ_1", true), make_creator(u8"ρ_2", false),
                         make_annihilator(u8"ρ_3", false)});

    using Term = decltype(hopping1);

    cout << "Kinetic terms in Hamiltonian\n";
    cout << '\t' << hopping1 << '\n';
    cout << '\t' << hopping2 << '\n';
    cout << '\t' << hopping3 << '\n';
    cout << '\t' << hopping4 << '\n';
    cout << '\n';

    cout << "Interaction terms in Hamiltonian\n";
    cout << '\t' << interaction << '\n';
    cout << '\n';

    cout << "One operator:\n";
    cout << "\tKinetic terms:\n";
    {
        vector<Term> comm;
        commutate(hopping1, t1, comm);
        commutate(hopping2, t1, comm);
        commutate(hopping3, t1, comm);
        commutate(hopping4, t1, comm);
        for(auto& t : comm) {
            t.prefactor.order_kroneckers();
            cout << "\t\t" << t << '\n';
        }
    }
    cout << '\n';

    cout << "\tInteraction terms:\n";
    {
        vector<Term> comm;
        commutate(interaction, t1, comm);
        for(auto& t : comm) {
            t.prefactor.order_kroneckers();
            cout << "\t\t" << t << '\n';
        }
    }
    cout << "\n\n";

    cout << "Three operators:\n";
    cout << "\tKinetic terms:\n";
    {
        vector<Term> comm;
        commutate(hopping1, t2, comm);
        commutate(hopping2, t2, comm);
        commutate(hopping3, t2, comm);
        commutate(hopping4, t2, comm);
        for(auto& t : comm) {
            t.prefactor.order_kroneckers();
            cout << "\t\t" << t << '\n';
        }
    }
    cout << '\n';

    cout << "\tInteraction terms:\n";
    {
        vector<Term> comm;
        commutate(interaction, t2, comm);
        for(auto& t : comm) {
            t.prefactor.order_kroneckers();
            cout << "\t\t" << t << '\n';
        }
    }
    return 0;
}
