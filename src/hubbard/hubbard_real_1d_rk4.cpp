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
    Application::name        = "hubbard_real_1d_rk4";
    Application::description = "Calculate <n_{0,↑}>(t) for the 1d Hubbard model";

    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("N", make_value<uint64_t>(16), "number of lattice sites")
        ("J", make_value<double>(1.), "hopping prefactor")
        ("U", make_value<double>(1.), "interaction strength")
        ("dt", make_value<double>(0.01), "step width of RK4 integrator")
        ("steps", make_value<uint64_t>(1000), "number of integrator steps")
        ("measurement_interval", make_value<uint64_t>(10), "interval between measurements")
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
    auto h = init_vector(app, basis);

    const auto rk4             = init_rk4(basis.size(), dt);
    const auto site_occupation = init_site_occupation_observable<Basis3>(lattice);

    double t = 0.;
    double n_ev;

    app.output_file << t << "\t" << 0.5 << "\n";

    for(; app.step < steps; ++app.step) {
        if((app.step % measurement_interval == 0ul) && (app.step > 0)) {
            get_loggers().main->info("Measuring at t={}", t);
            n_ev = site_occupation(basis, h);
            get_loggers().main->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev);
            app.output_file << t << '\t' << n_ev << '\n';
            app.output_file.flush();
            get_loggers().main->info("Finish measurement at t={}", t);
        }

        get_loggers().ode->info("Performing step {} of {} at t={}", app.step, steps, t);
        rk4.step(M, h);
        get_loggers().ode->info("Complete step {} -> {}", t, t + rk4.step_size());

        t += rk4.step_size();

        if(app.step % checkpoint_interval == 0ul) {
            write_checkpoint_file(app.checkpoint_prefix + std::to_string(app.step) + ".blaze", h);
        }
    }

    if(app.step % measurement_interval == 0ul) {
        get_loggers().main->info("Measuring at t={}", t);
        n_ev = site_occupation(basis, h);
        get_loggers().main->info(u8"  <n_{{0,↑}}>({}) = {}", t, n_ev);
        app.output_file << t << '\t' << n_ev << '\n';
        app.output_file.flush();
        get_loggers().main->info("Finish measurement at t={}", t);
    }

    return 0;
}
