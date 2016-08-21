#include <complex>
#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/hubbard/explicit_momentum_space.hpp>
#include <ieompp/agenda.hpp>
#include <ieompp/ieom/basis.hpp>
using namespace ieompp::algebra;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    auto program_desc = "This program computes the operator basis for the Hubbard "
                        "model on the 1D square lattice in momentum space."
                        "\n\nOptions";
    po::options_description desc(program_desc);
    desc.add_options()
        ("help", "print this help message")
        ("N", po::value<long>()->default_value(10), "set number of lattice sites")
        ("q", po::value<double>()->default_value(ieompp::HalfPi<double>::value), "set momentum of operator")
        ("J", po::value<double>()->default_value(1.), "set hopping prefactor")
        ("U", po::value<double>()->default_value(1.), "set interaction prefactor");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    const auto N = vm["N"].as<long>();
    auto q       = vm["q"].as<double>();
    const auto J = vm["J"].as<double>();
    const auto U = vm["U"].as<double>();
    cout << "N = " << N << endl;
    cout << "q = " << q << endl;
    cout << "J = " << J << endl;
    cout << "U = " << U << endl;
    cout << endl;

    ieompp::discretization::LinearDiscretization<double, long> momentum_space(N);
    ieompp::discretization::LinearDiscretization<double, long> lattice(N, 1.);
    auto q_idx = momentum_space(q);
    q = momentum_space[q_idx];

    cout << "using closest momentum value:" << endl;
    cout << "q     = " << q << endl;
    cout << "q_idx = " << q_idx << endl;
    cout << endl;

    auto term = make_term(std::complex<double>(1.), {make_creator(q_idx, true)});
    cout << "term  = " << term << endl;
    auto hamiltonian = ieompp::hubbard::momentum_space::Hamiltonian<double>{J, U};

    using Term = decltype(term);

    /* auto generator = [&hamiltonian, &momentum_space, &lattice](const Term& t, */
    /*                                                            std::vector<Term>& container) { */
    /*     /1* hamiltonian.generate_terms(t, momentum_space, lattice, container); *1/ */
    /*     hamiltonian.generate_interaction_terms(t, momentum_space, lattice, container); */
    /* }; */
    /* ieompp::ieom::Basis<Term> basis; */
    /* ieompp::ieom::create_basis(term, basis, generator, 1); */
    /* cout << basis.size() << endl; */
    /* for(auto& t : basis) cout << t << endl; */

    std::vector<Term> basis;
    basis.push_back(term);
    hamiltonian.generate_kinetic_terms_1(term, momentum_space, lattice, basis);
    cout << basis.size() << endl;
    for(auto& t : basis) cout << t << endl;


    /* using Term     = decltype(term); */
    /* auto generator = [&hamiltonian, &momentum_space, &lattice](const Term& t, */
    /*                                                            std::vector<Term>& container) { */
    /*     hamiltonian.generate_terms(t, momentum_space, lattice, container); */
    /* }; */

    /* auto agenda = ieompp::Agenda<decltype(term)>(); */
    /* agenda.commutate<std::vector<Term>>(term, 2, generator); */
}
