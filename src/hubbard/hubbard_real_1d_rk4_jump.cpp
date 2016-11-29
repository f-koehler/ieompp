#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

#include "include/real_space_1d.hpp"

#include <ieompp/models/hubbard_real_space.hpp>
namespace hubbard = ieompp::models::hubbard_real_space;

int main(int argc, char** argv)
{
    Application::name        = "hubbard_real_1d_rk4_jump";
    Application::description = "Calculate <Δn_{k,↑}>(t) for the 1d Hubbard model";
    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("N", make_value<uint64_t>(16), "number of lattice sites")
        ("J", make_value<double>(1.), "hopping prefactor")
        ("U", make_value<double>(1.), "interaction strength")
        ("dt", make_value<double>(0.01), "step width of RK4 integrator")
        ("t_end", make_value<double>(10), "stop time for simulation")
        ("measurement_interval", make_value<uint64_t>()->default_value(100), "interval between measurements in units of dt")
        ;
    // clang-format on

    Application app(argc, argv);

    const auto checkpoint_interval  = app.variables["checkpoint_interval"].as<uint64_t>();
    const auto N                    = app.variables["N"].as<uint64_t>();
    const auto J                    = app.variables["J"].as<double>();
    const auto U                    = app.variables["U"].as<double>();
    const auto dt                   = app.variables["dt"].as<double>();
    const auto t_end                = app.variables["t_end"].as<double>();
    const auto measurement_interval = app.variables["measurement_interval"].as<uint64_t>();

    // setting up a lattice
    Lattice lattice(N, 1.);

    // init operator basis
    const auto basis = init_basis<Basis3>(lattice);

    // compute matrix
    const auto L = hubbard::make_liouvillian(J, U);
    const auto M = compute_matrix(L, basis, lattice);
    write_matrix_file(app.matrix_path, M);

    // setting up initial vector
    auto h = init_vector(app, basis);

    // setup integrator
    const auto integrator = init_rk4(basis.size(), dt);

    // setup observable
    const auto fermi_jump = hubbard::FermiJump1D<double, Basis3>(
        lattice, hubbard::ExpectationValue1DHalfFilled<double, Lattice>{lattice});

    double jump, t = 0., last_measurement = 0., last_checkpoint = 0.;

    // write initial value of observable to file
    get_loggers().main->info("Measuring at t=0");
    jump = fermi_jump(basis, h);
    get_loggers().main->info(u8"  <Δn_{{k,↑}}>(0) = {}", jump);
    app.output_file << t << '\t' << jump << '\n';
    app.output_file.flush();
    get_loggers().main->info("Finish measurement at t=0");
    last_measurement = t;

    for(t = 0.; t < t_end;) {
        if(has_time_interval_passed(t, last_measurement, dt, measurement_interval)) {
            get_loggers().main->info("Measuring at t={}", t);
            jump = fermi_jump(basis, h);
            get_loggers().main->info(u8"  <Δn_{{k,↑}}>({}) = {}", t, jump);
            app.output_file << t << '\t' << jump << '\n';
            app.output_file.flush();
            get_loggers().main->info("Finish measurement at t={}", t);
            last_measurement = t;
        }

        get_loggers().ode->info("Integrate from t={}", t);
        integrator.step(M, h);
        get_loggers().ode->info("Finished integration t={} -> t={}", t, t + integrator.step_size());
        t += integrator.step_size();

        if(has_time_interval_passed(t, last_checkpoint, dt, checkpoint_interval)) {
            write_checkpoint_file(app.checkpoint_prefix, h, t);
            last_checkpoint = t;
        }
    }

    if(has_time_interval_passed(t, last_measurement, dt, measurement_interval)) {
        get_loggers().main->info("Measuring at t={}", t);
        jump = fermi_jump(basis, h);
        get_loggers().main->info(u8"  <Δn_{{k,↑}}>({}) = {}", t, jump);
        app.output_file << t << '\t' << jump << '\n';
        app.output_file.flush();
        get_loggers().main->info("Finish measurement at t={}", t);
        last_measurement = t;
    }

    return 0;
}
