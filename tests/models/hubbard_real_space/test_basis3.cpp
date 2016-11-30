#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>
using namespace ieompp;

using Operator = algebra::Operator<uint64_t, bool>;
using Monomial = algebra::Monomial<Operator>;

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
