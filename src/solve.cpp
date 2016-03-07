#include <iostream>
using namespace std;

#include "hubbard/discretization/linear.hpp"
#include "hubbard/algebra/hamiltonian.hpp"
#include "hubbard/algebra/normal_ordering.hpp"
#include "hubbard/io/termlist_file.hpp"
using namespace hubbard;

#include "quicli.hpp"
using namespace quicli;

int main()
{
    auto term =
        algebra::make_term(std::complex<double>(1., 0.), {algebra::make_creator(0ul, true)});
    hubbard::discretization::LinearDiscretization<double> discretization(100, 1.);
    hubbard::algebra::Hamiltonian<decltype(term)> hamiltonian;

    auto result = hamiltonian.commutate(term, discretization);
    hamiltonian.commutate(result, discretization, result);
    hamiltonian.commutate(result, discretization, result);
    hamiltonian.commutate(result, discretization, result);
    hamiltonian.commutate(result, discretization, result);
    hamiltonian.commutate(result, discretization, result);
    hamiltonian.commutate(result, discretization, result);
    result.sort();
    result.sum();
    cout << result.size() << endl;

    /* io::write("test.bin", result); */
    /* decltype(result) readin; */
    /* io::read("test.bin", readin); */
    /* assert(std::equal(readin.begin(), readin.end(), result.begin())); */
}
