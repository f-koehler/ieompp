#include <fstream>
#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/io/file_header.hpp>
#include <ieompp/io/eigen_sparse.hpp>
#include <ieompp/models/hubbard_explicit/matrix_elements.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/types/eigen_init.hpp>
using namespace ieompp::algebra;

#include <Eigen/Sparse>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    Eigen::initParallel();

    po::options_description description("Solve the 1d Hubbard model in real space\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("N", po::value<size_t>()->default_value(16), "number of lattice sites")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("U", po::value<double>()->default_value(.25), "interaction strength")
        ("out", po::value<string>()->default_value("real_1d.txt"), "output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << description << '\n';
        return 1;
    }

    const auto N     = vm["N"].as<size_t>();
    const auto J     = vm["J"].as<double>();
    const auto U     = vm["U"].as<double>();
    const auto out   = vm["out"].as<string>();

    ieompp::discretization::LinearDiscretization<double, size_t> lattice(N, 1.);

    using Operator = Operator<size_t, bool>;
    using Term     = Term<double, Operator>;

    // init operator basis
    vector<Term> basis;
    for(auto r : lattice) {
        basis.push_back(make_term(1., {make_creator(r, true)}));
    }
    for(auto r1 : lattice) {
        for(auto r2 : lattice) {
            for(auto r3 : lattice) {
                basis.push_back(make_term(1., {make_creator(r1, true), make_creator(r2, false),
                                               make_annihilator(r3, false)}));
            }
        }
    }
    const auto basis_size = basis.size();

    ieompp::hubbard::real_space::MatrixElements<double> elements{J, U};

    auto generator = [&elements, &basis, &lattice](std::size_t i, std::size_t j) {
        return elements.hopping(basis[i], basis[j], lattice)
               + elements.interaction(basis[i], basis[j]);
    };

    using Matrix = Eigen::SparseMatrix<double>;
    using Vector = Eigen::VectorXcd;

    Vector current(basis_size);
    current.setZero();
    current(0) = 1.;

    Matrix m;
    ofstream file(out.c_str());
    ieompp::io::write_header(file, {ieompp::get_description(ieompp::Platform()),
                                    ieompp::get_description(elements), ieompp::get_description(m)});
    ieompp::types::init(m, basis_size, basis_size, generator);
    cout << m << endl;
    ieompp::io::write_matrix(file, m);
    file.close();

    return 0;
}
