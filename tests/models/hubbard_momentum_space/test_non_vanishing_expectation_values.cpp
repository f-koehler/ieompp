#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>
#include <ieompp/models/hubbard_momentum_space/basis.hpp>
#include <ieompp/models/hubbard_momentum_space/non_vanishing_expectation_values.hpp>
#include <ieompp/types/number/is_equal.hpp>
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

namespace ieompp
{
    namespace models
    {
        namespace hubbard_momentum_space
        {
            bool operator==(
                const models::hubbard_momentum_space::NonVanishingExpectationValue<uint64_t,
                                                                                   double>& a,
                const models::hubbard_momentum_space::NonVanishingExpectationValue<uint64_t,
                                                                                   double>& b)
            {
                return (a.left_index == b.left_index) && (a.right_index == b.right_index)
                       && types::IsEqual(a.value, b.value);
            }
        }
    }
}

TEST_CASE("simple operators (half-filled, 1d)")
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

        models::hubbard_momentum_space::NonVanishingExpectationValues<uint64_t, double> nvevs(
            basis, conjugate_basis, dispersion, 0.);
        nvevs.sort();

        using Contribution =
            models::hubbard_momentum_space::NonVanishingExpectationValue<uint64_t, double>;

        REQUIRE(nvevs[0] == (Contribution{0ul, 0ul, 0.5}));
        REQUIRE(nvevs[1] == (Contribution{0ul, 4ul, 0.5}));
        REQUIRE(nvevs[2] == (Contribution{0ul, 5ul, 0.75}));
        REQUIRE(nvevs[3] == (Contribution{0ul, 6ul, 0.75}));
        REQUIRE(nvevs[4] == (Contribution{4ul, 0ul, 0.5}));
        REQUIRE(nvevs[5] == (Contribution{4ul, 4ul, 0.5}));
        REQUIRE(nvevs[6] == (Contribution{4ul, 5ul, 0.75}));
        REQUIRE(nvevs[7] == (Contribution{4ul, 6ul, 0.75}));
        REQUIRE(nvevs[8] == (Contribution{5ul, 0ul, 0.75}));
        REQUIRE(nvevs[9] == (Contribution{5ul, 4ul, 0.75}));
        REQUIRE(nvevs[10] == (Contribution{5ul, 5ul, 1.125}));
        REQUIRE(nvevs[11] == (Contribution{5ul, 6ul, 1.125}));
        REQUIRE(nvevs[12] == (Contribution{6ul, 0ul, 0.75}));
        REQUIRE(nvevs[13] == (Contribution{6ul, 4ul, 0.75}));
        REQUIRE(nvevs[14] == (Contribution{6ul, 5ul, 1.125}));
        REQUIRE(nvevs[15] == (Contribution{6ul, 6ul, 1.125}));
        REQUIRE(nvevs[16] == (Contribution{9ul, 9ul, 0.125}));
    }
}
