#include "include/application.hpp"
#include "include/common.hpp"
#include "include/rk4.hpp"
#include "include/vector.hpp"
#include "momentum_space/momentum_space_1d.hpp"
using namespace std;

#include "momentum_space/momentum_space_1d.hpp"

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
    const auto basis           = hubbard::Basis3Operator<Monomial>(N/2, brillouin_zone);
    const auto conjugate_basis = basis.get_conjugate();

    // computing matrix
    const auto L = hubbard::make_liouvillian(brillouin_zone, lattice, J, U);
    const auto M = compute_matrix(L, basis, lattice);
    write_matrix_file(app.matrix_path, M);

    // setting up initial vector
    auto h = init_vector(basis);

    hubbard::ParticleNumber<double, Basis3> obs(basis, conjugate_basis, L.dispersion, 0.);
    cout << basis[0] << '\n';
    cout << lattice[basis[0].front().index1] << '\n';
    cout << obs(h) << '\n';

    return 0;
}
