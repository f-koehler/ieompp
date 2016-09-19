#include <complex>
#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_explicit/matrix_elements.hpp>
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
        ("N", po::value<size_t>()->default_value(16), "set number of lattice sites")
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

    const auto N = vm["N"].as<size_t>();
    auto q       = vm["q"].as<double>();
    const auto J = vm["J"].as<double>();
    const auto U = vm["U"].as<double>();
    cout << "N = " << N << '\n';
    cout << "q = " << q << '\n';
    cout << "J = " << J << '\n';
    cout << "U = " << U << '\n';
    cout << '\n';

    ieompp::discretization::LinearDiscretization<double, size_t> momentum_space(N);
    ieompp::discretization::LinearDiscretization<double, size_t> lattice(N, 1.);
    auto q_idx = momentum_space(q);
    q = momentum_space[q_idx];

    cout << "using closest momentum value:\n";
    cout << "q     = " << q << '\n';
    cout << "q_idx = " << q_idx << '\n';
    cout << '\n';

    auto term = make_term(1., {make_creator(size_t(q_idx), true)});
    cout << "term  = " << term << '\n';
    ieompp::hubbard::momentum_space::MatrixElements<double> matrix_elements{J, U};

    cout << std::flush;

    const auto basis_size = N * N + 1ul;
    using Term = decltype(term);
    std::vector<Term> basis;
    basis.reserve(basis_size);
    basis.push_back(term);
    for(const auto k1_idx : momentum_space) {
        const auto k1 = momentum_space[k1_idx];
        for(const auto k2_idx : momentum_space) {
            const auto k2     = momentum_space[k2_idx];
            const auto k3_idx = momentum_space(k1 + k2 - q);
            basis.push_back(make_term(1., {make_creator(k1_idx, true), make_creator(k2_idx, false),
                                           make_annihilator(k3_idx, false)}));
        }
    }

    Eigen::MatrixXd m(basis_size, basis_size);
    for(auto i = 0l; i < basis_size; ++i) {
        for(auto j = 0l; j < basis_size; ++j) {
            m(i, j) = matrix_elements.hopping(basis[i], basis[j], momentum_space, lattice)
                      + matrix_elements.interaction(basis[i], basis[j], momentum_space);
        }
    }

    cout << "\n\n" << m << "\n\n";
}
