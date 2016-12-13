#include "basis3.hpp"
using namespace ieompp;

TEST_CASE("sort")
{
    std::vector<uint64_t> Ns = {8, 16, 32, 64};
    for(const auto N : Ns) {
        lattices::PeriodicChain<double, uint64_t> chain(N, 1.);
        models::hubbard_real_space::Basis3Operator<Monomial> basis(chain);
        const auto conjugate_basis = basis.get_conjugate();

        for(uint64_t i = 0; i < N; ++i) {
            REQUIRE(conjugate_basis[i] == Monomial{{Operator::make_annihilator(i, true)}});
        }

        for(uint64_t i = 0; i < N; ++i) {
            for(uint64_t j = 0; j < N; ++j) {
                for(uint64_t k = 0; k < N; ++k) {
                    REQUIRE(conjugate_basis[conjugate_basis.get_3op_index(i, j, k)]
                            == (Monomial{{Operator::make_creator(i, false),
                                          Operator::make_annihilator(j, false),
                                          Operator::make_annihilator(k, true)}}));
                }
            }
        }
    }
}
