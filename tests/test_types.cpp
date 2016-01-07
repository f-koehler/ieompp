#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sstream>
#include "hubbard/types.hpp"

TEST_CASE("format", "[types]") {
    SECTION("r == 0 && i == 0") {
        std::ostringstream strm;
        hubbard::format(strm, hubbard::Complex(0., 0.));
        REQUIRE(strm.str() == "0");
    }
}
