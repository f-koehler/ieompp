#include <fstream>
#include <iostream>
using namespace std;

#include <ieompp/io/eigen/sparse.hpp>
#include <ieompp/ode/rk4.hpp>
#include <ieompp/platform.hpp>
using namespace ieompp;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <spdlog/spdlog.h>
namespace spd = spdlog;

int main(int argc, char** argv)
{
    po::options_description description("Simply reads a sparse matrix file and integrates the ode "
                                        "system using RK4 with initial values (1,0,0,0,...)^T. The "
                                        "absolute value of the first entry will be written to a "
                                        "file.\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("in", po::value<string>(), "input file containing a sparse matrix")
        ("out", po::value<string>()->default_value("integrate_rk4.txt"), "output file")
        ("log", po::value<string>()->default_value("integrate_rk4.log"), "log file")
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

    const auto input_path   = vm["in"].as<string>();
    const auto output_path  = vm["out"].as<string>();
    const auto logging_path = vm["log"].as<string>();
    const auto dt           = vm["dt"].as<double>();
    const auto nnz          = vm["nnz"].as<size_t>();
    const auto t_end        = vm["t_end"].as<double>();

    vector<spd::sink_ptr> logging_sinks;
    logging_sinks.push_back(make_shared<spd::sinks::stderr_sink_st>());
    logging_sinks.push_back(make_shared<spd::sinks::simple_file_sink_st>(logging_path, true));
    auto logger = std::make_shared<spd::logger>("main", logging_sinks.begin(), logging_sinks.end());

    Eigen::SparseMatrix<std::complex<double>> M;

    logger->info("Open matrix file {} for reading", input_path);
    ifstream in_file(input_path.c_str());

    logger->info("Read matrix file {}", input_path);
    io::read_matrix(in_file, M, nnz);

    logger->info("Close matrix file {}", input_path);
    in_file.close();
    M *= std::complex<double>(0., 1.);

    logger->info("Set initial values");
    Eigen::VectorXcd h(M.rows());
    h.setZero();
    h(0) = 1.;

    logger->info("Open output file {} for writing", output_path);
    ofstream out_file (output_path.c_str());

    ode::RK4<double> integrator(M.rows(), dt);
    out_file << 0 << '\t' << h(0).real() << '\t' << h(0).imag() << '\n';
    for(double t = 0.; t < t_end;) {
        logger->info("Performing step at t={}", t);
        integrator.step(M, h);
        logger->info("Complete step {} -> {}", t, t + integrator.step_size());
        t += integrator.step_size();
        logger->info("\th_0({}) = {} + i {}", t, h(0).real(), h(0).imag());
        logger->info("\tstep_size = {}", integrator.step_size());
        out_file << t << '\t' << h(0).real() << '\t' << h(0).imag() << '\n';
    }

    logger->info("Close output file {}", output_path);
    out_file.close();

    return 0;
}
