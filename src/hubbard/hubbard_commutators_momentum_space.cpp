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
                              {make_creator("k", true), make_annihilator("k", true)});
    auto hopping2 = make_term(symbolic::make_prefactor(1.),
                              {make_creator("k", false), make_annihilator("k", false)});
    auto interaction = make_term(symbolic::make_prefactor(1.),
                                 {make_creator("k_1", true), make_annihilator("k_2", true),
                                  make_creator("k_3", false), make_annihilator("k_4", false)});
    auto t1 = make_term(symbolic::make_prefactor(1.), {make_creator("q", true)});
    auto t2 = make_term(
        symbolic::make_prefactor(1.),
        {make_creator("q_1", true), make_creator("q_2", false), make_annihilator("q_3", false)});

    using Term = decltype(hopping1);

    cout << "One operator:\n";
    cout << "\t[ " << hopping1 << " + " << hopping2 << "  ,  " << t1 << " ]=\n";
    {
        vector<Term> comm;
        commutate(hopping1, t1, comm);
        commutate(hopping2, t1, comm);
        for(auto& t : comm) {
            t.prefactor.order_kroneckers();
            cout << "\t\t" << t << '\n';
        }
    }
    cout << '\n';

    cout << "\t[ " << interaction << "  ,  " << t1 << " ]=\n";
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
    cout << "\t[ " << hopping1 << " + " << hopping2 << "  ,  " << t2 << " ]=\n";
    {
        vector<Term> comm;
        commutate(hopping1, t2, comm);
        commutate(hopping2, t2, comm);
        for(auto& t : comm) {
            t.prefactor.order_kroneckers();
            cout << "\t\t" << t << '\n';
        }
    }
    cout << '\n';

    cout << "\t[ " << interaction << "  ,  " << t2 << " ]=\n";
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
