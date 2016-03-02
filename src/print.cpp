#include <iostream>
using namespace std;

#include "hubbard/config.hpp"
#include "hubbard/algebra.hpp"
#include "hubbard/abstract.hpp"

int main()
{
    cout << "version:    \t" << hubbard::version << endl;
    cout << endl;

    cout << "algebra:" << endl;
    cout << "-------------------------------" << endl;
    cout << "creator:    \t" << hubbard::algebra::make_creator(0, true) << endl;
    cout << "annihilator:\t" << hubbard::algebra::make_annihilator(1, false) << endl;
    cout << "term:       \t"
         << hubbard::algebra::make_term(std::complex<double>(1., 2.),
                                        {hubbard::algebra::make_creator(0, true),
                                         hubbard::algebra::make_annihilator(1, false)})
         << endl;
    cout << endl;
    
    cout << "abstract:" << endl;
    cout << "-------------------------------" << endl;
    cout << "creator:    \t" << hubbard::abstract::make_creator("k_1", "σ") << endl;
    cout << "annihilator:\t" << hubbard::abstract::make_annihilator("k_2", "σ'") << endl;
    cout << "kronecker:  \t" << hubbard::abstract::Kronecker{"a", "b"} << endl;
    cout << "prefactor:  \t"
         << hubbard::abstract::AbstractPrefactor{hubbard::Complex{1., 2.},
                                                 {hubbard::abstract::Kronecker{"a", "b"}}}
         << endl;
    cout << "term:       \t"
         << hubbard::abstract::make_term(
                hubbard::abstract::AbstractPrefactor{hubbard::Complex{1., 2.},
                                                     {hubbard::abstract::Kronecker{"a", "b"}}},
                {hubbard::abstract::make_creator("k_1", "σ"),
                 hubbard::abstract::make_annihilator("k_2", "σ'")})
         << endl;
}
