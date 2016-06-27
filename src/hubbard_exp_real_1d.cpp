#include <complex>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

#include <ieompp/agenda.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/hubbard/explicit_real_space.hpp>
namespace algebra = ieompp::algebra;

using Complex = complex<double>;

int main()
{
    auto initial_term = algebra::make_term(Complex(1.), {algebra::make_creator(0, true)});
    auto lattice      = ieompp::discretization::LinearDiscretization<double, int>(10, 1.);
    auto hamiltonian  = ieompp::hubbard::real_space::Hamiltonian<double>{1., 1.};

    using Term = decltype(initial_term);
    ieompp::Agenda<Term> agenda;

    auto gen = [&](const Term& t, std::vector<Term>& c) {
        hamiltonian.generate_terms(t, lattice, c);
    };

    agenda.commutate<std::vector<Term>>(initial_term, 2, gen);
    cout << agenda << endl;
}
