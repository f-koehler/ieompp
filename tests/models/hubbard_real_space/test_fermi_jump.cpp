#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard/dispersion.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>
#include <ieompp/models/hubbard_real_space/expectation_value.hpp>
#include <ieompp/models/hubbard_real_space/fermi_jump.hpp>
using namespace ieompp;

using Operator = algebra::Operator<uint64_t, bool>;
using Monomial = algebra::Monomial<Operator>;
using Basis    = models::hubbard_real_space::Basis3Operator<Monomial>;

TEST_CASE("fourier coefficients")
{
    static const auto k_F = HalfPi<double>::value;

    for(uint64_t N = 3; N <= 32; ++N) {
        const lattices::PeriodicChain<double, uint64_t> lattice(N, 1.);
        const models::hubbard_real_space::ExpectationValue1DHalfFilled<double, decltype(lattice)>
            ev(lattice, 1., 0.5);
        const Basis basis(lattice);
        const models::hubbard_real_space::FermiJump1D<double, Basis> jump(
            basis, lattice,
            [&ev](const typename Basis::Monomial::Operator& a,
                  const typename Basis::Monomial::Operator& b) { return ev(a.index1, b.index1); },
            models::hubbard::calculate_fermi_momentum_1d(0.5));

        for(auto site : lattice) {
            REQUIRE(jump.fourier_coefficients()[site].real()
                    == Approx(std::cos(k_F * lattice[site])));
            REQUIRE(jump.fourier_coefficients()[site].imag()
                    == Approx(std::sin(k_F * lattice[site])));
        }
    }
}
