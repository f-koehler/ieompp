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
    result.sort();
    result.sum();
    cout << result.size() << endl;


    io::TermListFile<decltype(term)> file_bin("results.bin", true), file_txt("results.txt", false);
    file_bin.write(result);
    file_txt.write(result);

    auto read_txt = file_txt.read();
    assert(read_txt.size() == result.size());
    for(std::size_t i = 0; i < result.size(); ++i) {
        assert(result[i].same_operators(read_txt[i]));
    }
}
