#include "include/application.hpp"
#include "include/common.hpp"
#include "include/rk4.hpp"
#include "include/vector.hpp"
#include "real_space/basis1.hpp"
#include "real_space/expectation_value_1d.hpp"
#include "real_space/liouvillian.hpp"
#include "real_space/matrix.hpp"
#include "real_space/periodic_chain.hpp"
#include "real_space/site_occupation.hpp"
using namespace std;

namespace hubbard = ieompp::models::hubbard_real_space;

int main(int argc, char** argv)
{
    Application::name        = "hubbard_real_1d_kinetic_rk4";
    Application::description = "Calculate <n_{0,↑}>(t) for the 1d Hubbard model";
    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("N", make_value<uint64_t>(16), "number of lattice sites")
        ("J", make_value<double>(1.), "hopping prefactor")
        ("dt", make_value<double>(0.01), "step width of RK4 integrator")
        ("t_end", make_value<double>(10), "stop time for simulation")
        ("measurement_interval", make_value<uint64_t>()->default_value(100), "interval between measurements in units of dt")
        ("filling_factor", make_value<double>(0.5), "filling factor of the initial Fermi sea")
        ;
    // clang-format on

    Application app(argc, argv);

    const auto N                    = app.variables["N"].as<uint64_t>();
    const auto J                    = app.variables["J"].as<double>();
    const auto dt                   = app.variables["dt"].as<double>();
    const auto t_end                = app.variables["t_end"].as<double>();
    const auto measurement_interval = app.variables["measurement_interval"].as<uint64_t>();
    const auto filling_factor       = app.variables["filling_factor"].as<double>();

    const auto lattice         = init_lattice(N, 1.);
    const auto basis           = init_basis(lattice);
    const auto conjugate_basis = basis.get_conjugate();
    const auto ev              = init_expectation_value(lattice, filling_factor);
    const auto L               = init_liouvillian(J);
    const auto M               = compute_kinetic_matrix(L, basis, lattice);

    auto h                     = init_vector(basis);
    const auto integrator      = init_rk4(basis.size(), dt);
    const auto site_occupation = init_site_occupation(basis, conjugate_basis, ev);

    double obs, t, last_measurement;

    get_loggers().main->info("Measuring at t=0");
    obs = site_occupation(h);
    get_loggers().main->info(u8"  <n_{{0,↑}}>(0) = {}", obs);
    app.output_file << 0 << '\t' << obs << '\n';
    app.output_file.flush();
    get_loggers().main->info("Finish measurement at t=0");
    last_measurement = 0;

    for(t = 0.; t < t_end;) {
        if(has_time_interval_passed(t, last_measurement, dt, measurement_interval)) {
            get_loggers().main->info("Measuring at t={}", t);
            obs = site_occupation(h);
            get_loggers().main->info(u8"  <n_{{0,↑}}>({}) = {}", t, obs);
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
        obs = site_occupation(h);
        get_loggers().main->info(u8"  <n_{{0,↑}}>({}) = {}", t, obs);
        app.output_file << t << '\t' << obs << '\n';
        app.output_file.flush();
        get_loggers().main->info("Finish measurement at t={}", t);
        last_measurement = t;
    }

    return 0;
}
