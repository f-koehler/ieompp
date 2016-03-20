#include <iostream>
using namespace std;

#include "ieompp/config.hpp"
#include "ieompp/algebra.hpp"
#include "ieompp/abstract.hpp"

int main()
{
    cout << "version:    \t" << ieompp::version << endl;
    cout << endl;

    cout << "algebra:" << endl;
    cout << "-------------------------------" << endl;
    cout << "creator:    \t" << ieompp::algebra::make_creator(0, true) << endl;
    cout << "annihilator:\t" << ieompp::algebra::make_annihilator(1, false) << endl;
    cout << "term:       \t"
         << ieompp::algebra::make_term(std::complex<double>(1., 2.),
                                       {ieompp::algebra::make_creator(0, true),
                                        ieompp::algebra::make_annihilator(1, false)})
         << endl;
    cout << endl;

    cout << "abstract:" << endl;
    cout << "-------------------------------" << endl;
    cout << "creator:    \t" << ieompp::abstract::make_creator("k_1", "σ") << endl;
    cout << "annihilator:\t" << ieompp::abstract::make_annihilator("k_2", "σ'") << endl;
    cout << "kronecker:  \t" << ieompp::abstract::Kronecker{"a", "b"} << endl;
    cout << "prefactor:  \t"
         << ieompp::abstract::AbstractPrefactor{std::complex<double>(1., 2.),
                                                {ieompp::abstract::Kronecker{"a", "b"}}}
         << endl;
    cout << "term:       \t"
         << ieompp::abstract::make_term(
                ieompp::abstract::AbstractPrefactor{std::complex<double>(1., 2.),
                                                    {ieompp::abstract::Kronecker{"a", "b"}}},
                {ieompp::abstract::make_creator("k_1", "σ"),
                 ieompp::abstract::make_annihilator("k_2", "σ'")})
         << endl;
}
