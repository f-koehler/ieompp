#include <iostream>
using namespace std;

#include "hubbard/algebra.hpp"
#include "hubbard/square2d.hpp"
using namespace hubbard;
using namespace hubbard::square2d;

#include "quicli.hpp"
using namespace quicli;

int main(int argc, char** argv)
{
    /* CLI cli("solve"); */
    /* ValueMap vm; */

    /* cli.add(Parameter("--nx")).mandatory(true); */
    /* cli.add(Parameter("--ny")).mandatory(true); */
    /* cli.add(Parameter("--kx")).mandatory(true); */
    /* cli.add(Parameter("--ky")).mandatory(true); */
    /* cli.add(Parameter("--commutations")).default_value("2"); */

    /* auto args = quicli::convert(argc, argv); */
    /* try { */
    /*     cli.parse(args, vm); */
    /*     cli.validate(vm); */
    /* } catch(const std::exception& e) { */
    /*     cerr << e.what() << endl; */
    /*     std::exit(1); */
    /* } */

    /* auto disc = discretize(as<size_t>(vm.get("--nx")), as<size_t>(vm.get("--ny"))); */
    /* auto kx = disc.kx_index(as<double>(vm.get("--kx"))); */
    /* auto ky = disc.ky_index(as<double>(vm.get("--ky"))); */

    /* using Operator = algebra::Operator<std::size_t, bool>; */
    /* using Term     = algebra::Term<Operator, Complex>; */
    /* using TermList = algebra::TermList<Term>; */

    /* auto t1 = algebra::make_term(Complex(1., 0.), {algebra::make_creator(Momentum{kx, ky}, true)}); */
    /* for(std::size_t x = 0; x < disc.nx; ++x) { */
    /*     for(std::size_t y = 0; y < disc.ny; ++y) { */
    /*         auto t2 = algebra::make_term(Complex(1., 0.), */
    /*                                      { */
    /*                                          algebra::make_creator(Momentum{x, y}, true), */
    /*                                          algebra::make_annihilator(Momentum{x, y}, true), */
    /*                                      }); */
    /*         auto res = algebra::commutate(t1, t2); */
    /*         if(x == kx && y == ky) cout << res.front() << endl; */
    /*     } */
    /* } */
    /* auto k_min = -hubbard::PI; */
    /* auto k_max = hubbard::PI; */
    auto n     = 20000ul;

    using Operator = algebra::Operator<size_t, bool>;
    using Term     = algebra::Term<Operator, Complex>;
    using TermList = algebra::TermList<Term>;
    TermList list;
    auto t1 = algebra::make_term(Complex(1., 0.), {algebra::make_creator(150ul, true)});
    for(auto x1 = 0ul; x1 < n; ++x1) {
        for(auto x2 = 0ul; x2 < n; ++x2) {
            for(auto x3 = 0ul; x3 < n; ++x3) {
                for(auto x4 = 0ul; x4 < n; ++x4) {
                    auto t2 = algebra::make_term(
                        Complex(1., 0.),
                        {algebra::make_creator(x1, true), algebra::make_annihilator(x2, true),
                         algebra::make_creator(x3, true), algebra::make_annihilator(x4, false)});
                    algebra::commutate(t1, t2, list);
                }
            }
        }
    }
}
