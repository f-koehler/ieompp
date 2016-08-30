#include <iostream>
using namespace std;

#include <ieompp/algebra/commutator.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/types/number.hpp>
using namespace ieompp::algebra;

template <typename Term>
typename ieompp::types::real_type<typename Term::Prefactor>::type inner_product_1_1(const Term& a,
                                                                                    const Term& b)
{
    auto& op_a = a.operators.front();
    auto& op_b = b.operators.front();
    if((op_a.creator == op_b.creator) && (op_a.same_indices(op_b))) return 1.;
    return 0.;
}


int main()
{
    auto term1 = make_term(1., {make_annihilator(0, true)});
    auto term2 = make_term(1., {make_annihilator(0, true), make_annihilator(0, true), make_annihilator(0, true)});

    std::vector<decltype(term1)> comm;
    commutate(term1, term2, comm);

    for(auto& t : comm) {
        cout << t << '\n';
    }
}
