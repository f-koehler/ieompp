#include "include/application.hpp"
#include "include/common.hpp"
#include "include/rk4.hpp"
#include "include/vector.hpp"
#include "real_space/basis3.hpp"
#include "real_space/expectation_value_2d.hpp"
#include "real_space/liouvillian.hpp"
#include "real_space/matrix.hpp"
#include "real_space/periodic_square_lattice.hpp"
#include <ieompp/models/hubbard_real_space/fermi_jump/2d.hpp>
using namespace std;

namespace hubbard = ieompp::models::hubbard_real_space;

int main(int argc, char** argv)
{
    Application::name        = "hubbard_real_2d_rk4";
    Application::description = "Calculate <n_{0,↑}>(t) for the 2d Hubbard model";
    Application::add_default_options();

    // clang-format off
    Application::options_description.add_options()
        ("Nx", make_value<uint64_t>(4), "number of lattice sites in x direction")
        ("Ny", make_value<uint64_t>(4), "number of lattice sites in y direction")
        ("J", make_value<double>(1.), "hopping prefactor")
        ("U", make_value<double>(1.), "interaction strength")
        ("dt", make_value<double>(0.01), "step width of RK4 integrator")
        ("t_end", make_value<double>(10), "stop time for simulation")
        ("measurement_interval", make_value<uint64_t>()->default_value(100), "interval between measurements in units of dt")
        ("kx", make_value<double>()->default_value(ieompp::HalfPi<double>::value), "x component of the fermi momentum")
        ("ky", make_value<double>()->default_value(ieompp::HalfPi<double>::value), "y component of the fermi momentum")
        ;
    // clang-format on

    Application app(argc, argv);

    const auto Nx                   = app.variables["Nx"].as<uint64_t>();
    const auto Ny                   = app.variables["Ny"].as<uint64_t>();
    const auto J                    = app.variables["J"].as<double>();
    const auto U                    = app.variables["U"].as<double>();
    const auto dt                   = app.variables["dt"].as<double>();
    const auto t_end                = app.variables["t_end"].as<double>();
    const auto measurement_interval = app.variables["measurement_interval"].as<uint64_t>();
    const auto kx                   = app.variables["kx"].as<double>();
    const auto ky                   = app.variables["ky"].as<double>();

    const auto lattice         = init_lattice(Nx, Ny);
    const auto basis           = init_basis(lattice);
    const auto conjugate_basis = basis.get_conjugate();
    const auto ev              = init_expectation_value(lattice);
    const auto L               = init_liouvillian(J, U);
    const auto M               = compute_matrix(L, basis, lattice);
    const auto jump            = hubbard::FermiJump2D<double, Basis>(
        basis, lattice,
        [&ev](const typename Basis::Monomial::Operator& a,
              const typename Basis::Monomial::Operator& b) { return ev(a.index1, b.index1); },
        typename decltype(lattice)::Vector{kx, ky});

    auto h                     = init_vector(basis);
    const auto integrator      = init_rk4(basis.size(), dt);

    double obs, t, last_measurement = 0.;

    get_loggers().main->info("Measuring at t=0");
    obs = jump(h);
    get_loggers().main->info(u8"  Δn_{{k_F,↑}}(0) = {}", obs);
    app.output_file << 0 << '\t' << obs << '\n';
    app.output_file.flush();
    get_loggers().main->info("Finish measurement at t=0");
    last_measurement = 0;

    get_loggers().main->info("h[0] = {}", h[0]);

    for(t = 0.; t < t_end;) {
        if(has_time_interval_passed(t, last_measurement, dt, measurement_interval)) {
            get_loggers().main->info("Measuring at t={}", t);
            obs = jump(h);
            get_loggers().main->info(u8"  Δn_{{k_F,↑}}({}) = {}", t, obs);
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
        obs = jump(h);
        get_loggers().main->info(u8"  Δn_{{k_F,↑}}({}) = {}", t, obs);
        app.output_file << t << '\t' << obs << '\n';
        app.output_file.flush();
        get_loggers().main->info("Finish measurement at t={}", t);
        last_measurement = t;
    }

    return 0;
}
