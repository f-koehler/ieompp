#include "basis3.hpp"
using namespace ieompp;

TEST_CASE("init")
{
    lattices::PeriodicChain<double, uint64_t> chain(3, 1.);
    models::hubbard_real_space::Basis3Operator<Monomial> basis(chain);

    REQUIRE(basis[0] == Monomial{{Operator::make_creator(0, true)}});
    REQUIRE(basis[1] == Monomial{{Operator::make_creator(1, true)}});
    REQUIRE(basis[2] == Monomial{{Operator::make_creator(2, true)}});

    // clang-format off
    REQUIRE(basis[3]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(0, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[4]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(0, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[5]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(0, false), Operator::make_annihilator(2, false)}}));
    REQUIRE(basis[6]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(1, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[7]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(1, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[8]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(1, false), Operator::make_annihilator(2, false)}}));
    REQUIRE(basis[9]  == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(2, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[10] == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(2, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[11] == (Monomial{{Operator::make_creator(0, true), Operator::make_creator(2, false), Operator::make_annihilator(2, false)}}));

    REQUIRE(basis[12] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(0, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[13] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(0, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[14] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(0, false), Operator::make_annihilator(2, false)}}));
    REQUIRE(basis[15] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(1, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[16] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(1, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[17] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(1, false), Operator::make_annihilator(2, false)}}));
    REQUIRE(basis[18] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(2, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[19] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(2, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[20] == (Monomial{{Operator::make_creator(1, true), Operator::make_creator(2, false), Operator::make_annihilator(2, false)}}));

    REQUIRE(basis[21] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(0, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[22] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(0, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[23] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(0, false), Operator::make_annihilator(2, false)}}));
    REQUIRE(basis[24] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(1, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[25] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(1, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[26] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(1, false), Operator::make_annihilator(2, false)}}));
    REQUIRE(basis[27] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(2, false), Operator::make_annihilator(0, false)}}));
    REQUIRE(basis[28] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(2, false), Operator::make_annihilator(1, false)}}));
    REQUIRE(basis[29] == (Monomial{{Operator::make_creator(2, true), Operator::make_creator(2, false), Operator::make_annihilator(2, false)}}));
    // clang-format on
}
