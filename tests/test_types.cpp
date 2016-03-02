#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sstream>
#include "hubbard/types.hpp"

/* TEST_CASE("format", "[types]") */
/* { */
/*     using hubbard::Complex; */
/*     SECTION("r == 0 && i == 0") */
/*     { */
/*         std::ostringstream strm; */
/*         hubbard::format(strm, Complex(0., 0.)); */
/*         REQUIRE(strm.str() == "(0)"); */
/*     } */

/*     SECTION("r != 0 && i != 0") */
/*     { */
/*         std::ostringstream strm; */
/*         hubbard::format(strm, Complex(12., 4.)); */
/*         REQUIRE(strm.str() == "(12+4i)"); */
        
/*         strm = std::ostringstream(); */
/*         hubbard::format(strm, Complex(-12., -4.)); */
/*         REQUIRE(strm.str() == "(-12-4i)"); */
/*     } */

/*     SECTION("r == 0") */
/*     { */
/*         std::ostringstream strm; */
/*         hubbard::format(strm, Complex(0., 4.)); */
/*         REQUIRE(strm.str() == "(4i)"); */
/*     } */

/*     SECTION("i == 0") */
/*     { */
/*         std::ostringstream strm; */
/*         hubbard::format(strm, Complex(4., 0.)); */
/*         REQUIRE(strm.str() == "(4)"); */
/*     } */
/* } */
