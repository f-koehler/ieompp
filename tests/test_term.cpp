#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/algebra.hpp"

TEST_CASE("multiplication", "[algebra] [operator]") {
    using Operator = Operator<int, bool>;
    using Term = Term<Operator>;

    SECTION("Term * Operator") {
        Term t1;
        Operator o1(true, 1, true), o2(true, 0, false);

        auto t2 = t1*o1*o2;
        REQUIRE(t2[0] == o1);
        REQUIRE(t2[1] == o2);

        t1 *= t2;
        t1 *= o1;
        REQUIRE(t1[0] == o1);
        REQUIRE(t1[1] == o2);
        REQUIRE(t1[2] == o1);
    }

    SECTION("Term * Term") {
        Term t1, t2;
        Operator o1(true, 1, true), o2(true, 0, false);

        // TODO: check prefactor

        t1 *= o1;
        t1 *= o1;
        t1 *= o2;
        t2 *= o2;
        t2 *= o2;
        t2 *= o1;

        auto t3 = t1*t2;
        REQUIRE(t3[0] == o1);
        REQUIRE(t3[1] == o1);
        REQUIRE(t3[2] == o2);
        REQUIRE(t3[3] == o2);
        REQUIRE(t3[4] == o2);
        REQUIRE(t3[5] == o1);
        
        t1 *= t2;
        REQUIRE(t1[0] == o1);
        REQUIRE(t1[1] == o1);
        REQUIRE(t1[2] == o2);
        REQUIRE(t1[3] == o2);
        REQUIRE(t1[4] == o2);
        REQUIRE(t1[5] == o1);
    }
}
