#include <catch.hpp>

#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_square_lattice.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
using namespace ieompp;

TEST_CASE("ExpectationValue2DHalfFilled")
{
    const auto prefactor1 = 2. / (Pi<double>::value * Pi<double>::value);
    const auto prefactor2 = 1. / Pi<double>::value;
    const auto pi_half    = HalfPi<double>::value;

    for(uint64_t Nx = 3; Nx <= 16; ++Nx) {
        const auto max_dist_x = Nx / 2;

        for(uint64_t Ny = 3; Ny <= 16; ++Ny) {
            const auto may_dist_y = Ny / 2;

            const lattices::PeriodicSquareLattice<double, uint64_t> lattice(Nx, Ny, 1., 1.);
            models::hubbard_real_space::ExpectationValue2DHalfFilled<double, decltype(lattice)> ev(
                lattice);

            for(auto dist_x = 0ul; dist_x <= max_dist_x; ++dist_x) {
                for(auto dist_y = 0ul; dist_y <= may_dist_y; ++dist_y) {
                    const auto index          = lattice.index(dist_x, dist_y);
                    const auto lattice_dist_x = lattice.lattice_distance_x(0, index);
                    const auto lattice_dist_y = lattice.lattice_distance_y(0, index);

                    auto expectation = 0.5;
                    const auto sum   = lattice_dist_x + lattice_dist_y;
                    const auto diff  = (lattice_dist_x > lattice_dist_y)
                                          ? lattice_dist_x - lattice_dist_y
                                          : lattice_dist_y - lattice_dist_x;
                    if(lattice_dist_x != lattice_dist_y) {
                        expectation = prefactor1 * (std::sin(pi_half * sum) / sum)
                                      * (std::sin(pi_half * diff) / diff);
                    } else if(lattice_dist_x != 0) {
                        expectation = prefactor2 * std::sin(pi_half * sum) / sum;
                    }
                }
            }
        }
    }
}
