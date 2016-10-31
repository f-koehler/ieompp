#include <fstream>
#include <iostream>
using namespace std;

#include <ieompp/types/blaze.hpp>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/application_timer.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/io/blaze/sparse.hpp>
#include <ieompp/models/hubbard/basis.hpp>
#include <ieompp/models/hubbard/expectation_value.hpp>
#include <ieompp/models/hubbard/matrix_blaze.hpp>
#include <ieompp/models/hubbard/observable.hpp>
#include <ieompp/ode/rk4.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
using namespace ieompp;
namespace spd = spdlog;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    const ApplicationTimer timer;
    const std::string program_name("hubbard_kinetic_1d_real_rk4");

    po::options_description description("Calculate the matrix for the 1D Hubbard model on a linear "
                                        "lattice in real space and solve the ODE system using RK4 "
                                        "while writing site 0 occupation to file\n\nOptions");
    // clang-format off
    description.add_options()
        ("help", "print this help message")
        ("version", "print version information")
        ("N", po::value<uint64_t>()->default_value(16), "number of lattice sites")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("dt", po::value<double>()->default_value(0.01), "step width of RK4 integrator")
        ("steps", po::value<uint64_t>()->default_value(1000), "number of integrator steps")
        ("measurement_interval", po::value<uint64_t>()->default_value(10), "interval between measurements")
        ("flush_interval", po::value<uint64_t>()->default_value(100), "steps between flushes of output file")
        ("out", po::value<string>()->default_value(program_name + ".txt"), "output file")
        ("log", po::value<string>()->default_value(program_name + ".log"), "log file");
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help") != 0u) {
        cout << program_name << "\n\n";
        cout << description << '\n';
        return 1;
    }
    if(vm.count("version") != 0u) {
        cout << program_name << "\n\n";
        cout << get_type_description<Platform>() << '\n';
        return 1;
    }

    const auto N                    = vm["N"].as<uint64_t>();
    const auto J                    = vm["J"].as<double>();
    const auto dt                   = vm["dt"].as<double>();
    const auto steps                = vm["steps"].as<uint64_t>();
    const auto measurement_interval = vm["measurement_interval"].as<uint_fast64_t>();
    const auto flush_interval       = vm["flush_interval"].as<uint64_t>();
    const auto out_path             = vm["out"].as<string>();
    const auto log_path             = vm["log"].as<string>();

    // setting up logging facilities
    vector<spd::sink_ptr> logging_sinks;
    logging_sinks.push_back(make_shared<spd::sinks::stderr_sink_mt>());
    logging_sinks.push_back(make_shared<spd::sinks::simple_file_sink_mt>(log_path, true));
    auto main_logger =
        std::make_shared<spd::logger>("main", logging_sinks.begin(), logging_sinks.end());
    auto io_logger =
        std::make_shared<spd::logger>("io", logging_sinks.begin(), logging_sinks.end());
    auto hubbard_logger =
        std::make_shared<spd::logger>("hubbard", logging_sinks.begin(), logging_sinks.end());
    auto ode_logger =
        std::make_shared<spd::logger>("ode", logging_sinks.begin(), logging_sinks.end());

    main_logger->info("CLI options:");
    main_logger->info("  N   = {}", N);
    main_logger->info("  J   = {}", J);
    main_logger->info("  out = {}", out_path);
    main_logger->info("  log = {}", log_path);

    // setting up a lattice
    hubbard_logger->info("Setting up lattice");
    discretization::LinearDiscretization<double, uint64_t> lattice(N, 1.);
    log(hubbard_logger, get_description(lattice));

    using Operator = algebra::Operator<uint64_t, bool>;
    using Term     = algebra::Term<double, Operator>;

    // init operator basis
    using Basis = hubbard::real_space::Basis1Operator<Term>;
    hubbard_logger->info("Setting up operator basis");
    Basis basis(lattice);
    log(hubbard_logger, get_description(basis));

    // computing matrix
    hubbard_logger->info("Creating {}x{} sparse, complex matrix", basis.size(), basis.size());
    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    M.reserve(basis.size() * 10);
    hubbard_logger->info("Computing matrix elements");
    hubbard::real_space::init_kinetic_matrix(M, basis, lattice, J);
    hubbard_logger->info("  {} out of {} matrix elements are non-zero", M.nonZeros(),
                         M.rows() * M.columns());
    hubbard_logger->info("Multiply matrix with prefactor 1i");
    M *= std::complex<double>(0, 1);

    // setting up initial vector
    hubbard_logger->info("Setting up {} dimensional vector with initial conditions", basis.size());
    blaze::DynamicVector<std::complex<double>> h(basis.size());
    h.reset();
    h[0] = 1.;

    //
    io_logger->info("Open output file {}", out_path);
    ofstream out_file(out_path.c_str());
    ode::RK4<double> solver(basis.size(), dt);
    hubbard::real_space::ParticleNumber<decltype(basis)> observable{
        hubbard::real_space::ExpectationValue1DHalfFilled<double, decltype(lattice)>{lattice}};

    double t                     = 0.;
    uint64_t flush_counter       = 0;
    uint64_t measurement_counter = 0;

    hubbard_logger->info("Measuring at t={}", t);
    auto n_ev = observable(basis, h);
    hubbard_logger->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev);
    out_file << t << '\t' << n_ev.real() << '\t' << n_ev.imag() << '\n';
    hubbard_logger->info("Finish measurement at t={}", t);

    for(uint64_t step = 0; step < steps; ++step) {
        ode_logger->info("Performing step {} of {} at t={}", step, steps, t);
        solver.step(M, h);
        ode_logger->info("Complete step {} -> {}", t, t + solver.step_size());

        ++flush_counter;
        t += solver.step_size();

        if(measurement_counter % measurement_interval == 0ul) {
            hubbard_logger->info("Measuring at t={}", t);
            n_ev = observable(basis, h);
            hubbard_logger->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev);
            out_file << t << '\t' << n_ev.real() << '\t' << n_ev.imag() << '\n';
            hubbard_logger->info("Finish measurement at t={}", t);
        }

        if(flush_counter % flush_interval == 0ul) {
            io_logger->info("Flushing file {} ", out_path);
            out_file.flush();
            io_logger->info("Finish flushing file {}", out_path);
        }
    }
    io_logger->info("Close file {}", out_path);
    out_file.close();

    main_logger->info("Execution took {}", timer);

    return 0;
}
