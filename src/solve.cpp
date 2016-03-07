#include <iostream>
using namespace std;

#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra/hamiltonian.hpp"
#include "hubbard/algebra/normal_ordering.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    auto term = algebra::make_term(
        std::complex<double>(1., 0.),
        {
            algebra::make_creator(0ul, true), hubbard::algebra::make_creator(1ul, true),
        });
    hubbard::discretization::LinearDiscretization<double> discretization(100, 1.);
    hubbard::algebra::Hamiltonian<decltype(term)> hamiltonian;
    auto result = hamiltonian.commutate(term, discretization);
    hamiltonian.commutate(result, discretization, result);
    hubbard::algebra::sum_terms(result);

    for(auto& term : result)
    {
        cout << term << endl;
    }
}
