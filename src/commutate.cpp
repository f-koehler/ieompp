#include <iostream>
using namespace std;

#include <ieompp/algebra/commutator.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/symbolic/prefactor.hpp>
using namespace ieompp;
using namespace ieompp::algebra;

int main()
{
    auto t1 = make_term(symbolic::make_prefactor(1.),
                        {make_creator("k_1", true), make_annihilator("k_1", true)});
    auto t2 = make_term(symbolic::make_prefactor(1.), {make_creator("q_1", true)});
    vector<decltype(t1)> commutator;
    commutate(t1, t2, commutator);

    cout << "[" << t1 << ", " << t2 << "] = \n";
    for(const auto& t : commutator) {
        cout << t << '\n';
    }
    return 0;
}
