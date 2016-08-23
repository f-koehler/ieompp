#include <complex>
#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/hubbard/explicit_momentum_space.hpp>
#include <ieompp/ieom/basis.hpp>
#include <ieompp/ieom/dynamical_system.hpp>
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
        ("N", po::value<long>()->default_value(16), "set number of lattice sites")
        ("q", po::value<double>()->default_value(ieompp::HalfPi<double>::value), "set momentum of operator")
        ("J", po::value<double>()->default_value(1.), "set hopping prefactor")
        ("U", po::value<double>()->default_value(1.), "set interaction prefactor");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << desc << '\n';
        return 1;
    }

    const auto N = vm["N"].as<long>();
    auto q       = vm["q"].as<double>();
    const auto J = vm["J"].as<double>();
    const auto U = vm["U"].as<double>();
    cout << "N = " << N << '\n';
    cout << "q = " << q << '\n';
    cout << "J = " << J << '\n';
    cout << "U = " << U << '\n';
    cout << '\n';

    ieompp::discretization::LinearDiscretization<double, long> momentum_space(N);
    ieompp::discretization::LinearDiscretization<double, long> lattice(N, 1.);
    auto q_idx = momentum_space(q);
    q = momentum_space[q_idx];

    cout << "using closest momentum value:\n";
    cout << "q     = " << q << '\n';
    cout << "q_idx = " << q_idx << '\n';
    cout << '\n';

    auto term = make_term(std::complex<double>(1.), {make_creator(long(q_idx), true)});
    cout << "term  = " << term << '\n';
    auto hamiltonian = ieompp::hubbard::momentum_space::Hamiltonian<double>{J, U};

    using Term = decltype(term);

    auto generator = [&hamiltonian, &momentum_space, &lattice](const Term& t,
                                                               std::vector<Term>& container) {
        hamiltonian.generate_terms(t, momentum_space, lattice, container);
    };

    ieompp::ieom::DynamicalSystem<Term> dyn;
    dyn.basis = ieompp::ieom::make_basis(term, generator, 2);
    cout << "basis size = " << dyn.basis.size() << '\n';
    dyn.create_matrix(generator);
    cout << dyn.matrix << '\n';
}
