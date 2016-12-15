#include "excited_fermi_sea_1d.hpp"
using namespace ieompp;

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard/dispersion.hpp>
#include <ieompp/models/hubbard_momentum_space/basis.hpp>

TEST_CASE("3-operator basis (1d, half-filled)")
{
    const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;

    SECTION("N=3")
    {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(3);
        const auto lattice        = lattices::PeriodicChain<double, uint64_t>(3, 1.);
        const auto dispersion     = models::hubbard::make_dispersion(brillouin_zone, lattice, 1.);

        const auto basis =
            models::hubbard_momentum_space::Basis3Operator<Monomial>(1, brillouin_zone);
        const auto conjugate_basis = basis.get_conjugate();

        std::vector<std::pair<uint64_t, uint64_t>> non_vanishing{{{0ul, 0ul},
                                                                  {0ul, 5ul},
                                                                  {0ul, 6ul},
                                                                  {5ul, 0ul},
                                                                  {5ul, 5ul},
                                                                  {5ul, 6ul},
                                                                  {6ul, 0ul},
                                                                  {6ul, 5ul},
                                                                  {6ul, 6ul},
                                                                  {9ul, 9ul}}};

        for(uint64_t i = 0; i < basis.size(); ++i) {
            for(uint64_t j = 0; j < basis.size(); ++j) {
                models::hubbard_momentum_space::ExcitedFermiSea<Monomial> state;
                state.apply_monomial(conjugate_basis[j], dispersion, 0.);
                state.apply_monomial(basis[i], dispersion, 0.);

                auto pos = std::find(non_vanishing.begin(), non_vanishing.end(),
                                     std::pair<uint64_t, uint64_t>{i, j});
                if(pos != non_vanishing.end()) {
                    REQUIRE(state.is_initial_fermi_sea());
                } else {
                    REQUIRE(!state.is_initial_fermi_sea());
                }
            }
        }
    }
}
