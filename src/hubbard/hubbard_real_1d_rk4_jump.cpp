#include "include/application.hpp"
#include "include/common.hpp"
#include "include/rk4.hpp"
#include "include/vector.hpp"
#include "real_space/basis3.hpp"
#include "real_space/expectation_value_1d.hpp"
#include "real_space/fermi_jump.hpp"
#include "real_space/liouvillian.hpp"
#include "real_space/matrix.hpp"
#include "real_space/periodic_chain.hpp"
using namespace std;

namespace hubbard = ieompp::models::hubbard_real_space;

int main(int argc, char** argv)
{
    Application::name        = "hubbard_real_1d_rk4_jump";
    Application::description = "Calculate <n_{0,↑}>(t) for the 1d Hubbard model";
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

    const auto N                    = app.variables["N"].as<uint64_t>();
    const auto J                    = app.variables["J"].as<double>();
    const auto U                    = app.variables["U"].as<double>();
    const auto dt                   = app.variables["dt"].as<double>();
    const auto t_end                = app.variables["t_end"].as<double>();
    const auto measurement_interval = app.variables["measurement_interval"].as<uint64_t>();

    const auto lattice = init_lattice(N, 1.);
    const auto basis   = init_basis(lattice);
    const auto ev      = init_expectation_value(lattice);
    const auto L       = init_liouvillian(J, U);
    const auto M       = compute_matrix(L, basis, lattice);

    auto h                = init_vector(basis);
    const auto integrator = init_rk4(basis.size(), dt);
    const auto fermi_jump = init_fermi_jump(basis, lattice, ev);

    double obs, t, last_measurement = 0.;

    get_loggers().main->info("Measuring at t=0");
    obs = fermi_jump(basis, h);
    get_loggers().main->info(u8"  <Δn_{{k_F,↑}}>(0) = {}", obs);
    app.output_file << 0 << '\t' << obs << '\n';
    app.output_file.flush();
    get_loggers().main->info("Finish measurement at t=0");
    last_measurement = 0;

    for(t = 0.; t < t_end;) {
        if(has_time_interval_passed(t, last_measurement, dt, measurement_interval)) {
            get_loggers().main->info("Measuring at t={}", t);
            obs = fermi_jump(basis, h);
            get_loggers().main->info(u8"  <Δn_{{k_F,↑}}>({}) = {}", t, obs);
            app.output_file << t << '\t' << obs << '\n';
            app.output_file.flush();
            get_loggers().main->info("Finish measurement at t={}", t);
            last_measurement = t;
        }

        get_loggers().ode->info("Integrate from t={}", t);
        integrator.step(M, h);
        get_loggers().ode->info("Finished integration t={} -> t={}", t, t + integrator.step_size());
        t += integrator.step_size();
    }

    if(has_time_interval_passed(t, last_measurement, dt, measurement_interval)) {
        get_loggers().main->info("Measuring at t={}", t);
        obs = fermi_jump(basis, h);
        get_loggers().main->info(u8"  <Δn_{{k_F,↑}}>({}) = {}", t, obs);
        app.output_file << t << '\t' << obs << '\n';
        app.output_file.flush();
        get_loggers().main->info("Finish measurement at t={}", t);
        last_measurement = t;
    }

    return 0;
}
