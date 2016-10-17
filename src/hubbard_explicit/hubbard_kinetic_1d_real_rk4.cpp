#include <iostream>
#include <fstream>
using namespace std;

#include <ieompp/types/blaze.hpp>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/io/blaze/sparse.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/models/hubbard_explicit/expectation_values.hpp>
#include <ieompp/models/hubbard_explicit/matrix_blaze.hpp>
#include <ieompp/ode/rk4.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
using namespace ieompp;
namespace spd = spdlog;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    const std::string program_name("hubbard_matrix_1d_real");

    po::options_description description("Calculate the matrix for the 1D Hubbard model on a linear "
                                        "lattice in real space and solve the ODE system using RK4 "
                                        "while writing site 0 occupation to file\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("version", "print version information")
        ("N", po::value<size_t>()->default_value(16), "number of lattice sites")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("dt", po::value<double>()->default_value(0.01), "step width of RK4 integrator")
        ("t_end", po::value<double>()->default_value(10.), "stop time of integration")
        ("out", po::value<string>()->default_value("hubbard_1d_real_rk4.txt"), "output file")
        ("log", po::value<string>()->default_value("hubbard_1d_real_rk4.log"), "log file");

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
    const auto dt       = vm["dt"].as<double>();
    const auto t_end    = vm["t_end"].as<double>();
    const auto out_path = vm["out"].as<string>();
    const auto log_path = vm["log"].as<string>();

    // setting up logging facilities
    vector<spd::sink_ptr> logging_sinks;
    logging_sinks.push_back(make_shared<spd::sinks::stderr_sink_st>());
    logging_sinks.push_back(make_shared<spd::sinks::simple_file_sink_st>(log_path, true));
    auto main_logger = std::make_shared<spd::logger>("main", logging_sinks.begin(), logging_sinks.end());
    auto io_logger = std::make_shared<spd::logger>("io", logging_sinks.begin(), logging_sinks.end());
    auto hubbard_logger = std::make_shared<spd::logger>("hubbard", logging_sinks.begin(), logging_sinks.end());
    auto ode_logger = std::make_shared<spd::logger>("ode", logging_sinks.begin(), logging_sinks.end());

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

    // computing matrix
    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    M.reserve(basis.size() * 10);
    hubbard_logger->info("Computing matrix elements");
    hubbard::real_space::init_kinetic_matrix(M, basis, lattice, J);
    hubbard_logger->info("  {} out of {} matrix elements are non-zero", M.nonZeros(),
                         M.rows() * M.columns());
    M *= std::complex<double>(0, 1);

    // setting up initial vector
    blaze::DynamicVector<std::complex<double>> h(basis.size());
    h.reset();
    h[0] = 1.;

    //
    ofstream out_file(out_path.c_str());
    ode::RK4<double> solver(basis.size(), dt);
    hubbard::real_space::SiteOccupation1Op<decltype(lattice)> occ(0, lattice);
    for(auto t = 0.; t < t_end; t += dt) {
        auto tmp = occ(h);
        out_file << t << '\t' << tmp.real() << '\t' << tmp.imag() << '\n';
        ode_logger->info("Performing step at t={}", t);
        solver.step(M, h);
        ode_logger->info("Complete step {} -> {}", t, t + solver.step_size());
    }
    out_file.close();

    return 0;
}
