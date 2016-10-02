#include <fstream>
#include <iostream>
using namespace std;

#include <ieompp/io/eigen_sparse.hpp>
#include <ieompp/ode/rk4.hpp>
using namespace ieompp;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description description("Simply reads a sparse matrix file and integrates the ode "
                                        "system using RK4 with initial values (1,0,0,0,...)^T. The "
                                        "absolute value of the first entry will be written to a "
                                        "file.\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("i", po::value<string>(), "input file containing a sparse matrix")
        ("o", po::value<string>()->default_value("integrate_rk4.txt"), "output file")
        ("dt", po::value<double>()->default_value(.01), "step width")
        ("nnz", po::value<size_t>()->default_value(8), "number of non zero elements per row")
        ("t_end", po::value<double>()->default_value(10.), "stop time for the integration");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << description << '\n';
        return 1;
    }

    const auto input_path  = vm["i"].as<string>();
    const auto output_path = vm["o"].as<string>();
    const auto dt          = vm["dt"].as<double>();
    const auto nnz         = vm["nnz"].as<size_t>();
    const auto t_end       = vm["t_end"].as<double>();

    Eigen::SparseMatrix<std::complex<double>> M;
    ifstream in_file(input_path.c_str());
    io::read_matrix(in_file, M, nnz);
    in_file.close();
    M *= std::complex<double>(0., 1.);

    Eigen::VectorXcd h(M.rows());
    h.setZero();
    h(0) = 1.;

    ofstream out_file (output_path.c_str());
    ode::RK4<double> integrator(M.rows(), dt);
    for(double t = 0.; t < t_end;) {
        integrator.step(M, h);
        t += integrator.step_width();
        auto val = abs(h(0));
        out_file << t << '\t' << h(0).real() << '\t' << h(0).imag() << '\n';
    }
    out_file.close();

    return 0;
}
