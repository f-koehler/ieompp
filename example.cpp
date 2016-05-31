#include <iostream>
using namespace std;

#include "ieompp/term.hpp"
#include "ieompp/operator.hpp"
#include "ieompp/commutator.hpp"
#include "ieompp/symbolic/index.hpp"
#include "ieompp/symbolic/prefactor.hpp"

int main()
{
    using Index     = ieompp::symbolic::MomentumIndex<int>;
    using Spin      = ieompp::symbolic::SpinIndex<int>;
    using Prefactor = ieompp::symbolic::Prefactor<double>;

    auto term = ieompp::make_term(ieompp::symbolic::make_prefactor(1.),
                                  {ieompp::make_creator(Index{1}, Spin{1})});
    auto kin = ieompp::make_term(
        ieompp::symbolic::make_prefactor(1.),
        {ieompp::make_creator(Index{2}, Spin{2}), ieompp::make_annihilator(Index{2}, Spin{2})});
    auto interaction = ieompp::make_term(
        ieompp::symbolic::make_prefactor(1.),
        {ieompp::make_creator(Index{2}, Spin{2}), ieompp::make_annihilator(Index{3}, Spin{2}),
         ieompp::make_creator(Index{3}, Spin{3}), ieompp::make_annihilator(Index{4}, Spin{3})});

    using Term = decltype(term);
    using Operator = typename Term::Operator;
    using TermList = std::vector<Term>;

    TermList result;
    ieompp::commutate(kin, term, result);
    cout << "[" << kin << ", " << term << "] = ";
    std::copy(result.begin(), --result.end(), std::ostream_iterator<Term>(cout, " + "));
    cout << result.back();
    cout << endl;

    result.clear();
    ieompp::commutate(interaction, term, result);
    cout << "[" << interaction << ", " << term << "] = ";
    std::copy(result.begin(), --result.end(), std::ostream_iterator<Term>(cout, " + "));
    cout << result.back();
    cout << endl;
}
