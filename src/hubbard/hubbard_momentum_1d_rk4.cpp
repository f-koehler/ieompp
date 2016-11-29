#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

#include "include/momentum_space_1d.hpp"

namespace hubbard = ieompp::models::hubbard_momentum_space;

int main(int argc, char** argv)
{
    Application::name        = "hubbard_momentum_1d_rk4";
    Application::description = "Calculate <n_{k,↑}>(t) for the 1d Hubbard model";
    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("N", make_value<uint64_t>()->default_value(16), "number of points in brillouin zone discretization")
        ("J", make_value<double>()->default_value(1.), "hopping prefactor")
        ("U", make_value<double>()->default_value(1.), "interaction strength")
        ("dt", make_value<double>()->default_value(0.01), "step width of RK4 integrator")
        ("t_end", make_value<double>(10), "stop time for simulation")
        ("measurement_interval", make_value<double>()->default_value(0.1), "interval between measurements")
        ;
    // clang-format on

    Application app(argc, argv);

    /* const auto checkpoint_interval  = app.variables["checkpoint_interval"].as<uint64_t>(); */
    const auto N = app.variables["N"].as<uint64_t>();
    const auto J = app.variables["J"].as<double>();
    const auto U = app.variables["U"].as<double>();
    /* const auto dt                   = app.variables["dt"].as<double>(); */
    /* const auto t_end                = app.variables["t_end"].as<double>(); */
    /* const auto measurement_interval = app.variables["measurement_interval"].as<double>(); */

    // setting up lattice and brillouin_zone
    BrillouinZone brillouin_zone(N);
    Lattice lattice(N, 1.);

    // init operator basis
    const auto basis = init_basis<Basis3>(brillouin_zone);

    // computing matrix
    const auto L = hubbard::make_liouvillian(brillouin_zone, lattice, J, U);
    const auto M = compute_matrix(L, basis, lattice);
    write_matrix_file(app.matrix_path, M);

    // setting up initial vector
    auto h = init_vector(app, basis);

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

    hubbard::ParticleNumber<double, Basis3> obs(basis, L.dispersion, 0.);
    cout << obs(h) << '\n';

    return 0;
}
