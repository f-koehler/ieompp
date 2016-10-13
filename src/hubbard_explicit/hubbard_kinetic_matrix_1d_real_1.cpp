#include <iostream>
#include <fstream>
using namespace std;

#include <ieompp/types/blaze.hpp>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/io/blaze/sparse.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/models/hubbard_explicit/matrix_blaze.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
using namespace ieompp;
namespace spd = spdlog;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    const std::string program_name("hubbard_kinetic_matrix_1d_real_1");

    po::options_description description("Calculate the matrix for the 1D Hubbard model on a linear lattice in real space\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("version", "print version information")
        ("N", po::value<size_t>()->default_value(16), "number of lattice sites")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("out", po::value<string>()->default_value("hubbard_kinetic_matrix_1d_real_1.blaze"), "output file")
        ("log", po::value<string>()->default_value("hubbard_kinetic_matrix_1d_real_1.log"), "log file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << description << '\n';
        return 1;
    }
    if(vm.count("version")) {
        cout << program_name << "\n\n";
        cout << get_type_description<Platform>() << '\n';
        return 1;
    }

    const auto N        = vm["N"].as<size_t>();
    const auto J        = vm["J"].as<double>();
    const auto out_path = vm["out"].as<string>();
    const auto log_path = vm["log"].as<string>();

    // setting up logging facilities
    vector<spd::sink_ptr> logging_sinks;
    logging_sinks.push_back(make_shared<spd::sinks::stderr_sink_mt>());
    logging_sinks.push_back(make_shared<spd::sinks::simple_file_sink_mt>(log_path, true));
    auto main_logger = std::make_shared<spd::logger>("main", logging_sinks.begin(), logging_sinks.end());
    auto io_logger = std::make_shared<spd::logger>("io", logging_sinks.begin(), logging_sinks.end());
    auto hubbard_logger = std::make_shared<spd::logger>("hubbard", logging_sinks.begin(), logging_sinks.end());

    main_logger->info("CLI options:");
    main_logger->info("  N   = {}", N);
    main_logger->info("  J   = {}", J);
    main_logger->info("  out = {}", out_path);
    main_logger->info("  log = {}", log_path);

    // setting up a lattice
    hubbard_logger->info("setting up a lattice");
    discretization::LinearDiscretization<double, size_t> lattice(N, 1.);
    log(hubbard_logger, get_description(lattice));

    using Operator = algebra::Operator<size_t, bool>;
    using Term     = algebra::Term<double, Operator>;

    // init operator basis
    hubbard_logger->info("Setting up operator basis");
    hubbard::real_space::Basis1Operator<Term> basis(lattice);
    log(hubbard_logger, get_description(basis));

    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    M.reserve(basis.size() * 10);
    hubbard_logger->info("Computing matrix elements");
    hubbard::real_space::init_kinetic_matrix(M, basis, lattice, J);
    hubbard_logger->info("  {} out of {} matrix elements are non-zero", M.nonZeros(),
                         M.rows() * M.columns());

    io_logger->info("Write matrix file", M.nonZeros());
    io::write_matrix(out_path, M);

    return 0;
}
