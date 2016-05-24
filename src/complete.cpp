#include <iostream>
#include <complex>
using namespace std;

#include "ieompp/algebra/operator.hpp"
#include "ieompp/algebra/term.hpp"
#include "ieompp/algebra/commutator.hpp"
#include "ieompp/hubbard/hamiltonian.hpp"
#include "ieompp/discretization/linear.hpp"
#include "ieompp/ieom/normalization.hpp"
using namespace ieompp;

int main()
{
    using Operator = algebra::Operator<int, bool>;
    using Real     = double;
    using Complex  = complex<Real>;
    using Term     = algebra::Term<Operator, Complex>;

    // create initial term
    Term initial = algebra::make_term(Complex(1.), {algebra::make_creator(0, true)});

    // create a spatial discretization
    discretization::LinearDiscretization<Real, int> discretization(100, 1.);

    // create hamiltonian
    hubbard::Hamiltonian<Term> hamiltonian;

    // perform first commutation
    auto result_1_unordered = hamiltonian.commutate(initial, discretization);
    auto result_1           = algebra::order_operators(result_1_unordered);
    algebra::remove_vanishing_terms(result_1);
    algebra::remove_forbidden(result_1);
    result_1_unordered.clear();

    // perform normalization
    decltype(result_1) normalized_1;
    for(auto& t : result_1) ieom::normalize(t, normalized_1);

    // print result_1
    cout << initial << ":" << endl;
    for(auto& t : normalized_1) cout << "\t" << t << endl;
    cout << endl;

    // perform second commutation for each term
    for(auto& term : normalized_1) {
        auto result_unordered = hamiltonian.commutate(term, discretization);
        auto result = algebra::order_operators(result_unordered);
        algebra::remove_vanishing_terms(result);
        algebra::remove_forbidden(result);

        decltype(result) normalized;
        for(auto& t : result) ieom::normalize(t, normalized);

        cout << term << endl;
        for(auto& t : normalized) cout << "\t" << t << endl;
        cout << endl;
    }
}
