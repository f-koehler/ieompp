#include "basis3.hpp"
using namespace ieompp;

TEST_CASE("get_3op_index (explicit)")
{
    lattices::PeriodicChain<double, uint64_t> chain(3, 1.);
    models::hubbard_real_space::Basis3Operator<Monomial> basis(chain);

    REQUIRE(basis.get_3op_index(0, 0, 0) == 3);
    REQUIRE(basis.get_3op_index(0, 0, 1) == 4);
    REQUIRE(basis.get_3op_index(0, 0, 2) == 5);
    REQUIRE(basis.get_3op_index(0, 1, 0) == 6);
    REQUIRE(basis.get_3op_index(0, 1, 1) == 7);
    REQUIRE(basis.get_3op_index(0, 1, 2) == 8);
    REQUIRE(basis.get_3op_index(0, 2, 0) == 9);
    REQUIRE(basis.get_3op_index(0, 2, 1) == 10);
    REQUIRE(basis.get_3op_index(0, 2, 2) == 11);

    REQUIRE(basis.get_3op_index(1, 0, 0) == 12);
    REQUIRE(basis.get_3op_index(1, 0, 1) == 13);
    REQUIRE(basis.get_3op_index(1, 0, 2) == 14);
    REQUIRE(basis.get_3op_index(1, 1, 0) == 15);
    REQUIRE(basis.get_3op_index(1, 1, 1) == 16);
    REQUIRE(basis.get_3op_index(1, 1, 2) == 17);
    REQUIRE(basis.get_3op_index(1, 2, 0) == 18);
    REQUIRE(basis.get_3op_index(1, 2, 1) == 19);
    REQUIRE(basis.get_3op_index(1, 2, 2) == 20);

    REQUIRE(basis.get_3op_index(2, 0, 0) == 21);
    REQUIRE(basis.get_3op_index(2, 0, 1) == 22);
    REQUIRE(basis.get_3op_index(2, 0, 2) == 23);
    REQUIRE(basis.get_3op_index(2, 1, 0) == 24);
    REQUIRE(basis.get_3op_index(2, 1, 1) == 25);
    REQUIRE(basis.get_3op_index(2, 1, 2) == 26);
    REQUIRE(basis.get_3op_index(2, 2, 0) == 27);
    REQUIRE(basis.get_3op_index(2, 2, 1) == 28);
    REQUIRE(basis.get_3op_index(2, 2, 2) == 29);
}

TEST_CASE("get_3op_index")
{
    std::vector<uint64_t> Ns = {8, 16, 32, 64};
    for(const auto N : Ns) {
        lattices::PeriodicChain<double, uint64_t> chain(N, 1.);
        models::hubbard_real_space::Basis3Operator<Monomial> basis(chain);

        for(uint64_t i = 0; i < N; ++i) {
            REQUIRE(basis[i] == Monomial{{Operator::make_creator(i, true)}});
        }

        for(uint64_t i = 0; i < N; ++i) {
            for(uint64_t j = 0; j < N; ++j) {
                for(uint64_t k = 0; k < N; ++k) {
                    REQUIRE(basis[basis.get_3op_index(i, j, k)]
                            == (Monomial{{Operator::make_creator(i, true),
                                          Operator::make_creator(j, false),
                                          Operator::make_annihilator(k, false)}}));
                }
            }
        }
    }
}
