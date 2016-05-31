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

    auto term1 = ieompp::make_term(ieompp::symbolic::make_prefactor(1.),
                                   {ieompp::make_creator(Index(1, "q"), Spin(1, "s"))});
    auto term2 = ieompp::make_term(ieompp::symbolic::make_prefactor(1.),
                                   {ieompp::make_creator(Index(1, "q"), Spin(1, "s")),
                                    ieompp::make_creator(Index(2, "q"), Spin(2, "s")),
                                    ieompp::make_annihilator(Index(3, "q"), Spin(2, "s"))});
    auto kin = ieompp::make_term(
        ieompp::symbolic::make_prefactor(1.),
        {ieompp::make_creator(Index{1}, Spin{1}), ieompp::make_annihilator(Index{1}, Spin{1})});
    auto interaction = ieompp::make_term(
        ieompp::symbolic::make_prefactor(1.),
        {ieompp::make_creator(Index{1}, Spin{1}), ieompp::make_annihilator(Index{1}, Spin{1}),
         ieompp::make_creator(Index{2}, Spin{2}), ieompp::make_annihilator(Index{2}, Spin{2})});

    using Term     = decltype(term1);
    using Operator = typename Term::Operator;
    using TermList = std::vector<Term>;

    TermList result;
    ieompp::commutate(kin, term1, result);
    cout << "[" << kin << ", " << term1 << "] = ";
    std::copy(result.begin(), --result.end(), std::ostream_iterator<Term>(cout, " + "));
    cout << result.back();
    cout << endl;

    result.clear();
    ieompp::commutate(interaction, term1, result);
    cout << "[" << interaction << ", " << term1 << "] = ";
    std::copy(result.begin(), --result.end(), std::ostream_iterator<Term>(cout, " + "));
    cout << result.back();
    cout << endl;

    result.clear();
    ieompp::commutate(kin, term2, result);
    cout << "[" << kin << ", " << term2 << "] = ";
    std::copy(result.begin(), --result.end(), std::ostream_iterator<Term>(cout, " + "));
    cout << result.back();
    cout << endl;

    result.clear();
    ieompp::commutate(interaction, term2, result);
    cout << "[" << interaction << ", " << term2 << "] = ";
    std::copy(result.begin(), --result.end(), std::ostream_iterator<Term>(cout, " + "));
    cout << result.back();
    cout << endl;
}
