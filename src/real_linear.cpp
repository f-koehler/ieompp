#include <complex>
#include <iostream>
#include <fstream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/hubbard/explicit_real_space.hpp>
#include <ieompp/ieom/basis.hpp>
#include <ieompp/ieom/dynamical_system.hpp>
#include <ieompp/ieom/rk4.hpp>
using namespace ieompp::algebra;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    auto program_desc = "This program computs the operator basis for the Hubbard model on the 1D "
                        "square lattice in real space.\n\nOptions";
    po::options_description desc(program_desc);
    desc.add_options()
        ("help", "print this help message")
        ("N", po::value<long>()->default_value(16), "number of lattice sites")
        ("r_idx", po::value<long>()->default_value(0), "spatial index of initial operator")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("U", po::value<double>()->default_value(1.), "interaction prefactor")
        ("dt", po::value<double>()->default_value(0.01), "time step for ODE solver");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    const auto N     = vm["N"].as<long>();
    const auto r_idx = vm["r_idx"].as<long>();
    const auto J     = vm["J"].as<double>();
    const auto U     = vm["U"].as<double>();
    const auto dt    = vm["dt"].as<double>();
    cout << "N     = " << N << endl;
    cout << "r_idx = " << r_idx << endl;
    cout << "J     = " << J << endl;
    cout << "U     = " << U << endl;
    cout << "dt    = " << dt << endl;
    cout << endl;

    ieompp::discretization::LinearDiscretization<double, long> lattice(N, 1.);

    auto term = make_term(std::complex<double>(1.), {make_creator(r_idx, true)});
    cout << "term = " << term << endl;
    auto hamiltonian = ieompp::hubbard::real_space::Hamiltonian<double>{J, U};

    using Term = decltype(term);
    auto generator = [&hamiltonian, &lattice](const Term& t, std::vector<Term>& container) {
        hamiltonian.generate_terms(t, lattice, container);
    };

    ieompp::ieom::DynamicalSystem<Term> dyn;
    dyn.basis = ieompp::ieom::make_basis(term, generator, 2);
    cout << "basis size = " << dyn.basis.size() << endl;
    dyn.create_matrix(generator);
    cout << endl << dyn.matrix << endl << endl;

    using Solver = ieompp::ieom::RK4<Term>;
    Solver solver;
    solver.dt = dt;

    Solver::Vector v = Solver::Vector::Zero(4);
    v(0) = 1.;

    std::ofstream strm("real.txt");
    for(std::size_t i = 0; i < 10000; ++i) {
        solver.step(dyn.matrix, v);
        strm << i * dt << "\t" << std::abs(v(0)) << "\t" << std::abs(v(1)) << "\t" << std::abs(v(2))
             << "\t" << std::abs(v(3)) << endl;
    }
    strm.close();
}
