#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
#include <ieompp/models/hubbard_real_space/site_occupation.hpp>
using namespace ieompp;

using Operator = algebra::Operator<uint64_t, bool>;
using Monomial = algebra::Monomial<Operator>;
using Basis    = models::hubbard_real_space::Basis3Operator<Monomial>;

TEST_CASE("")
{
    const uint64_t N = 5;
    const lattices::PeriodicChain<double, uint64_t> lattice(N, 1.);
    const models::hubbard_real_space::ExpectationValue1DHalfFilled<double, decltype(lattice)> ev(
        lattice);
    const Basis basis(lattice);
    const auto conjugate_basis = basis.get_conjugate();

    const auto site_occupation = ieompp::models::hubbard_real_space::SiteOccupation<double, Basis>{
        [&ev](const typename Basis::Monomial::Operator& a,
              const typename Basis::Monomial::Operator& b) { return ev(a.index1, b.index1); },
        std::cref(basis), std::cref(conjugate_basis)};

    for(uint64_t i = 0; i < N; ++i) {
        for(uint64_t j = 0; j < N; ++j) {
            REQUIRE(site_occupation.expectation_value_1_1(basis[i], conjugate_basis[j])
                    == Approx(2 * ev(basis[i][0].index1, conjugate_basis[j][0].index1)));
        }
    }

    // clang-format off
    REQUIRE(site_occupation.expectation_value_3_1(basis[basis.get_3op_index(0, 0, 0)], conjugate_basis[0])
            == Approx(4 * ev(0, 0) * ev(0, 0) - 2 * ev(0, 0)));
    REQUIRE(site_occupation.expectation_value_3_1(basis[basis.get_3op_index(0, 1, 2)], conjugate_basis[3])
            == Approx(4 * ev(0, 3) * ev(1, 2)));
    REQUIRE(site_occupation.expectation_value_3_1(basis[basis.get_3op_index(0, 1, 1)], conjugate_basis[2])
            == Approx(4 * ev(0, 2) * ev(1, 1) - 2 * ev(0, 2)));
    REQUIRE(site_occupation.expectation_value_3_1(basis[basis.get_3op_index(0, 0, 1)], conjugate_basis[1])
            == Approx(4 * ev(0, 1) * ev(0, 1)));
    REQUIRE(site_occupation.expectation_value_3_1(basis[basis.get_3op_index(0, 0, 1)], conjugate_basis[0])
            == Approx(4 * ev(0, 0) * ev(0, 1)));
    // clang-format on
}
