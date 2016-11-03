#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

#include "program.hpp"


#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/application_timer.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard/basis.hpp>
#include <ieompp/models/hubbard/expectation_value.hpp>
#include <ieompp/models/hubbard/liouvillian_blaze.hpp>
#include <ieompp/models/hubbard/observable.hpp>
#include <ieompp/ode/rk4.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
using namespace ieompp;
namespace spd = spdlog;

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    const ApplicationTimer timer;
    const string program_name("hubbard_real_1d_rk4");

    po::options_description description("Calculate the matrix for the 1D Hubbard model on a linear "
                                        "momentum_space in real space and solve the ODE system using RK4 "
                                        "while writing site 0 occupation to file\n\nOptions");
    add_default_options(description);

    // clang-format off
    description.add_options()
        ("out", po::value<string>()->default_value(program_name + ".txt"), "output file")
        ("log", po::value<string>()->default_value(program_name + ".log"), "log file")
        ("N", po::value<uint64_t>()->default_value(16), "number of momentum_space sites")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("U", po::value<double>()->default_value(1.), "interaction strength")
        ("dt", po::value<double>()->default_value(0.01), "step width of RK4 integrator")
        ("steps", po::value<uint64_t>()->default_value(1000), "number of integrator steps")
        ("measurement_interval", po::value<uint64_t>()->default_value(10), "interval between measurements")
        ("flush_interval", po::value<uint64_t>()->default_value(100), "steps between flushes of output file")
        ;
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help") != 0u) {
        cout << program_name << " [options]" << '\n';
        cout << program_name << " [options] --checkpoint=<path>" << '\n';
        cout << '\n';
        cout << description << '\n';
        return 1;
    }

    if(vm.count("version") != 0u) {
        cout << program_name << "\n\n";
        write_platform_info(cout);
        return 1;
    }

    if(vm.count("response_file") != 0u) {
        read_response_file(vm["response_file"].as<string>(), vm, description);
    }

    const auto checkpoint_interval  = vm["checkpoint_interval"].as<uint64_t>();
    const auto N                    = vm["N"].as<uint64_t>();
    const auto J                    = vm["J"].as<double>();
    const auto U                    = vm["U"].as<double>();
    const auto dt                   = vm["dt"].as<double>();
    const auto steps                = vm["steps"].as<uint64_t>();
    const auto measurement_interval = vm["measurement_interval"].as<uint_fast64_t>();
    const auto flush_interval       = vm["flush_interval"].as<uint64_t>();
    const auto out_path             = vm["out"].as<string>();

    write_platform_info(cout);
    cout << flush;

    const auto rsp_path          = fs::change_extension(out_path, ".rsp").string();
    const auto log_path          = fs::change_extension(out_path, ".log").string();
    const auto matrix_path       = fs::change_extension(out_path, "").string() + "_matrix.blaze";
    const auto checkpoint_prefix = fs::change_extension(out_path, "").string() + "_checkpoint_";

    Loggers loggers(log_path);
    loggers.main->info("CLI options:");
    loggers.main->info("  N   = {}", N);
    loggers.main->info("  J   = {}", J);
    loggers.main->info("  U   = {}", U);
    loggers.main->info("  out = {}", out_path);

    write_response_file(rsp_path, argc, argv, loggers);

    // setting up lattice
    discretization::LinearDiscretization<double, uint64_t> momentum_space(N);
    discretization::LinearDiscretization<double, uint64_t> lattice(N, 1.);

    using Operator = algebra::Operator<uint64_t, bool>;
    using Term     = algebra::Term<double, Operator>;

    // init operator basis
    using Basis = hubbard::momentum_space::Basis3Operator<Term>;
    loggers.main->info("Setting up operator basis");
    Basis basis(0, momentum_space);

    // init dispersion
    hubbard::Dispersion<decltype(lattice)> dispersion(momentum_space, lattice, J);

    hubbard::momentum_space::Liouvillian<double> L{U};

    // computing matrix
    loggers.main->info("Creating {}x{} sparse, complex matrix", basis.size(), basis.size());
    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    M.reserve(basis.size() * 10);
    loggers.main->info("Computing matrix elements");
    L.init_matrix(M, basis, momentum_space, lattice, dispersion);
    loggers.main->info("  {} out of {} matrix elements are non-zero", M.nonZeros(),
                       M.rows() * M.columns());
    loggers.main->info("Multiply matrix with prefactor 1i");
    M *= std::complex<double>(0, 1);

    write_matrix_file(matrix_path, M, loggers);

    // setting up initial vector
    loggers.main->info("Setting up {} dimensional vector with initial conditions", basis.size());
    blaze::DynamicVector<std::complex<double>> h(basis.size());

    uint64_t initial_step = 0;
    if(vm.count("checkpoint") == 0) {
        h.reset();
        h[0] = 1.;
    } else {
        read_checkpoint_file(vm["checkpoint"].as<string>(), h, loggers);
        const regex re_checkpoint_file("^.*" + checkpoint_prefix + R"((\d+)\.blaze$)");
        smatch m;
        regex_match(vm["checkpoint"].as<string>(), m, re_checkpoint_file);
        initial_step = strtoul(m[1].str().c_str(), nullptr, 10);
        clean_output_file(out_path, initial_step);
    }

    //
    loggers.io->info("Open output file {}", out_path);
    ofstream out_file(out_path.c_str());
    write_platform_info(out_file);

    ode::RK4<double> solver(basis.size(), dt);
    /* hubbard::real_space::ParticleNumber<decltype(basis)> observable{ */
    /*     hubbard::real_space::ExpectationValue1DHalfFilled<double, decltype(momentum_space)>{momentum_space}}; */

    double t = 0.;

    /* loggers.main->info("Measuring at t={}", t); */
    /* auto n_ev = observable(basis, h); */
    /* loggers.main->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev); */
    /* out_file << t << '\t' << n_ev.real() << '\t' << n_ev.imag() << '\n'; */
    /* loggers.main->info("Finish measurement at t={}", t); */

    /* for(uint64_t step = initial_step; step < steps; ++step) { */
    /*     loggers.ode->info("Performing step {} of {} at t={}", step, steps, t); */
    /*     solver.step(M, h); */
    /*     loggers.ode->info("Complete step {} -> {}", t, t + solver.step_size()); */

    /*     t += solver.step_size(); */

    /*     if(step % checkpoint_interval == 0ul) { */
    /*         write_checkpoint_file(checkpoint_prefix + std::to_string(step) + ".blaze", h, loggers); */
    /*     } */

    /*     if(step % measurement_interval == 0ul) { */
    /*         /1* loggers.main->info("Measuring at t={}", t); *1/ */
    /*         /1* n_ev = observable(basis, h); *1/ */
    /*         /1* loggers.main->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev); *1/ */
    /*         /1* out_file << t << '\t' << n_ev.real() << '\t' << n_ev.imag() << '\n'; *1/ */
    /*         /1* loggers.main->info("Finish measurement at t={}", t); *1/ */
    /*     } */

    /*     if(step % flush_interval == 0ul) { */
    /*         loggers.io->info("Flushing file {} ", out_path); */
    /*         out_file.flush(); */
    /*         loggers.io->info("Finish flushing file {}", out_path); */
    /*     } */
    /* } */
    /* loggers.io->info("Close file {}", out_path); */
    /* out_file.close(); */

    /* loggers.main->info("Execution took {}", timer); */

    return 0;
}
