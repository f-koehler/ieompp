#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/abstract/prefactor.hpp"

using namespace ieompp;
using namespace abstract;

TEST_CASE("same_kroneckers", "[abstract]")
{
    SECTION("true case")
    {
        AbstractPrefactor a{std::complex<double>(-1., 2.),
                            {Kronecker{"i", "j"}, Kronecker{"s", u8"σ"}}};
        AbstractPrefactor b{std::complex<double>(3., 4.15),
                            {Kronecker{"j", "i"}, Kronecker{"s", u8"σ"}}};
        REQUIRE(a.same_kroneckers(b));
        REQUIRE(b.same_kroneckers(a));
    }
}
