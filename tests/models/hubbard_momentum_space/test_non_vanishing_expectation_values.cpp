#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>
#include <ieompp/models/hubbard_momentum_space/basis.hpp>
#include <ieompp/models/hubbard_momentum_space/non_vanishing_expectation_values.hpp>
using namespace ieompp;

using Monomial = algebra::Monomial<algebra::Operator<uint64_t, bool>>;

namespace Catch
{
    template <>
    struct StringMaker<std::pair<uint64_t, uint64_t>> {
        static std::string convert(const std::pair<uint64_t, uint64_t>& p)
        {
            std::ostringstream strm;
            strm << '{' << p.first << ", " << p.second << '}';
            return strm.str();
        }
    };
}

TEST_CASE("simple operators, half-filled, 1d")
{
    const auto kF_1 = -HalfPi<double>::value, kF_2 = HalfPi<double>::value;

    SECTION("N=3")
    {
        const auto brillouin_zone = lattices::PeriodicChain<double, uint64_t>(3);
        const auto lattice        = lattices::PeriodicChain<double, uint64_t>(3, 1.);
        const auto dispersion =
            models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

        const auto basis =
            models::hubbard_momentum_space::Basis3Operator<Monomial>(1, brillouin_zone);
        const auto conjugate_basis = basis.get_conjugate();

        models::hubbard_momentum_space::NonVanishingExpectationValues<uint64_t> nvevs(
            basis, conjugate_basis, dispersion, 0.);
        nvevs.sort();

        REQUIRE(nvevs.size() == 10);
        REQUIRE(nvevs[0] == std::make_pair(0ul, 0ul));
        REQUIRE(nvevs[1] == std::make_pair(0ul, 5ul));
        REQUIRE(nvevs[2] == std::make_pair(0ul, 6ul));
        REQUIRE(nvevs[3] == std::make_pair(5ul, 0ul));
        REQUIRE(nvevs[4] == std::make_pair(5ul, 5ul));
        REQUIRE(nvevs[5] == std::make_pair(5ul, 6ul));
        REQUIRE(nvevs[6] == std::make_pair(6ul, 0ul));
        REQUIRE(nvevs[7] == std::make_pair(6ul, 5ul));
        REQUIRE(nvevs[8] == std::make_pair(6ul, 6ul));
        REQUIRE(nvevs[9] == std::make_pair(9ul, 9ul));
    }
}
