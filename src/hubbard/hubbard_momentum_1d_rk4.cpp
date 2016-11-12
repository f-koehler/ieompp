#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

#include "momentum_space_1d.hpp"

namespace hubbard = ieompp::models::hubbard_momentum_space;

int main(int argc, char** argv)
{
    Application::name        = "hubbard_momentum_1d_rk4";
    Application::description = "Calculate <n_{k,↑}>(t) for the 1d Hubbard model";
    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("N", Value<uint64_t>()->default_value(16), "number of momentum_space sites")
        ("J", Value<double>()->default_value(1.), "hopping prefactor")
        ("U", Value<double>()->default_value(1.), "interaction strength")
        ("dt", Value<double>()->default_value(0.01), "step width of RK4 integrator")
        ("steps", Value<uint64_t>()->default_value(1000), "number of integrator steps")
        ("measurement_interval", Value<uint64_t>()->default_value(10), "interval between measurements")
        ;
    // clang-format on

    Application app(argc, argv);

    const auto checkpoint_interval  = app.variables["checkpoint_interval"].as<uint64_t>();
    const auto N                    = app.variables["N"].as<uint64_t>();
    const auto J                    = app.variables["J"].as<double>();
    const auto U                    = app.variables["U"].as<double>();
    const auto dt                   = app.variables["dt"].as<double>();
    const auto steps                = app.variables["steps"].as<uint64_t>();
    const auto measurement_interval = app.variables["measurement_interval"].as<uint_fast64_t>();

    get_loggers().main->info("CLI options:");
    get_loggers().main->info("  N   = {}", N);
    get_loggers().main->info("  J   = {}", J);
    get_loggers().main->info("  U   = {}", U);
    get_loggers().main->info("  out = {}", app.output_path);

    write_response_file(app.response_path, argc, argv);

    // setting up lattice
    MomentumSpace momentum_space(N);
    Lattice lattice(N, 1.);

    // init operator basis
    get_loggers().main->info("Setting up operator basis");
    Basis3 basis(0, momentum_space);

    // computing matrix
    const auto L = hubbard::make_liouvillian(momentum_space, lattice, J, U);
    const auto M = compute_matrix(L, basis, lattice);
    write_matrix_file(app.matrix_path, M);

    // setting up initial vector
    get_loggers().main->info("Setting up {} dimensional vector with initial conditions",
                             basis.size());
    blaze::DynamicVector<std::complex<double>> h(basis.size());

    uint64_t initial_step = 0;
    if(app.variables.count("checkpoint") == 0) {
        h.reset();
        h[0] = 1.;
    } else {
        read_checkpoint_file(app.variables["checkpoint"].as<string>(), h);
        const regex re_checkpoint_file("^.*" + app.checkpoint_prefix + R"((\d+)\.blaze$)");
        smatch m;
        regex_match(app.variables["checkpoint"].as<string>(), m, re_checkpoint_file);
        initial_step = strtoul(m[1].str().c_str(), nullptr, 10);
        clean_output_file(app.output_path, initial_step);
    }
    get_loggers().main->info("Finished setting up initial conditions");

    const auto rk4 = init_rk4(basis.size(), dt);

    return 0;
}
