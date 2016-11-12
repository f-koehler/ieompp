#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

#include "real_space_1d.hpp"

#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
#include <ieompp/models/hubbard_real_space/site_occupation.hpp>
namespace hubbard = ieompp::models::hubbard_real_space;

int main(int argc, char** argv)
{
    Application::name = "hubbard_real_1d_rk4";
    Application::description = "Calculate <n_{0,↑}>(t) for the 1d Hubbard model";
    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("N", Value<uint64_t>()->default_value(16), "number of lattice sites")
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

    // setting up a lattice
    Lattice lattice(N, 1.);

    // init operator basis
    get_loggers().main->info("Setting up operator basis");
    Basis3 basis(lattice);

    // compute matrix
    const auto L = hubbard::make_liouvillian(J, U);
    const auto M = compute_matrix(L, basis, lattice);
    write_matrix_file(app.matrix_path, M);

    // setting up initial vector
    get_loggers().main->info("Setting up {} dimensional vector with initial conditions", basis.size());
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

    ieompp::ode::RK4<double> solver(basis.size(), dt);
    hubbard::SiteOccupation<Basis3> observable{
        hubbard::ExpectationValue1DHalfFilled<double, decltype(lattice)>{lattice}};

    double t = 0.;
    double n_ev;

    for(uint64_t step = initial_step; step < steps; ++step) {
        if(step % measurement_interval == 0ul) {
            get_loggers().main->info("Measuring at t={}", t);
            n_ev = observable(basis, h);
            get_loggers().main->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev);
            app.output_file << t << '\t' << n_ev << '\n';
            app.output_file.flush();
            get_loggers().main->info("Finish measurement at t={}", t);
        }

        get_loggers().ode->info("Performing step {} of {} at t={}", step, steps, t);
        solver.step(M, h);
        get_loggers().ode->info("Complete step {} -> {}", t, t + solver.step_size());

        t += solver.step_size();

        if(step % checkpoint_interval == 0ul) {
            write_checkpoint_file(app.checkpoint_prefix + std::to_string(step) + ".blaze", h);
        }
    }

    if(steps % measurement_interval == 0ul) {
        get_loggers().main->info("Measuring at t={}", t);
        n_ev = observable(basis, h);
        get_loggers().main->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev);
        app.output_file << t << '\t' << n_ev << '\n';
        get_loggers().main->info("Finish measurement at t={}", t);
    }

    return 0;
}
