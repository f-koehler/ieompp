#include <complex>
#include <functional>
#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/fermi_sea.hpp>
#include <ieompp/hubbard/dispersion.hpp>
#include <ieompp/discretization/linear.hpp>
namespace algebra = ieompp::algebra;
using Complex = complex<double>;

int main()
{
    auto initial_term = algebra::make_term(
        Complex(1.), {algebra::make_creator(0, true), algebra::make_annihilator(0, true)});
    ieompp::FermiSea<decltype(initial_term)> fermi_sea;
    ieompp::discretization::LinearDiscretization<double, int> lattice(10, 1.);
    ieompp::discretization::LinearDiscretization<double, int> fourier_space(10000);

    initial_term.operators.front().index1 = fourier_space.closest(3.);
    initial_term.operators.back().index1  = fourier_space.closest(3.);
    cout << initial_term << endl;

    auto dispersion =
        bind(ieompp::hubbard::Dispersion<double, decltype(lattice)>{1.}, placeholders::_1, lattice);
    cout << fermi_sea.expectation_value_vanishes(initial_term, fourier_space, dispersion) << endl;
}
