#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ieompp/lattices/square.hpp>
using namespace ieompp;

#define VEC_APPROX(a, b) ((Approx(a[0]) == b[0]) && (Approx(a[1]) == b[1]))

const std::size_t NX = 100;
const std::size_t NY = 100;
const std::size_t N  = NX * NY;

template <typename Real>
void test_initialization_real_space()
{
    lattices::SquareDiscretization<Real> disc(NX, NY, 1., 1.);

    REQUIRE(disc.end() - disc.begin() == N);
    REQUIRE(disc.cend() - disc.cbegin() == N);

    REQUIRE(disc.size() == N);
    REQUIRE(disc.first() == 0);
    REQUIRE(disc.last() == N - 1);
    REQUIRE(disc.x_min() == Approx(0.));
    REQUIRE(disc.x_max() == Approx(Real(NX - 1)));
    REQUIRE(disc.x_length() == Approx(Real(NX)));
    REQUIRE(disc.dx() == Approx(1.));
    REQUIRE(disc.y_min() == Approx(0.));
    REQUIRE(disc.y_max() == Approx(Real(NY - 1)));
    REQUIRE(disc.y_length() == Approx(Real(NY)));
    REQUIRE(disc.dy() == Approx(1.));
    REQUIRE(disc.lattice_vectors().size() == 2);
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[0],
                       (typename lattices::SquareDiscretization<Real>::Vector{1., 0.})));
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[1],
                       (typename lattices::SquareDiscretization<Real>::Vector{0., 1.})));

    for(std::size_t i = 0; i < NX; ++i) {
        for(std::size_t j = 0; j < NY; ++j) {
            const auto idx = disc.index(i, j);
            REQUIRE(disc.begin()[idx] == idx);
            REQUIRE(VEC_APPROX(disc[idx], (typename lattices::SquareDiscretization<Real>::Vector{
                                              Real(i), Real(j)})));
            REQUIRE(disc(disc[idx]) == idx);
        }
    }
}

template <typename Real>
void test_initialization_momentum_space()
{
    lattices::SquareDiscretization<Real> disc(NX, NY);

    REQUIRE(disc.size() == N);
    REQUIRE(disc.first() == 0);
    REQUIRE(disc.last() == N - 1);
    REQUIRE(disc.x_min() == Approx(-Pi<Real>::value));
    REQUIRE(disc.x_max() == Approx(Pi<Real>::value));
    REQUIRE(disc.x_length() == Approx(TwoPi<Real>::value));
    REQUIRE(disc.dx() == Approx(TwoPi<Real>::value / NX));
    REQUIRE(disc.y_min() == Approx(-Pi<Real>::value));
    REQUIRE(disc.y_max() == Approx(Pi<Real>::value));
    REQUIRE(disc.y_length() == Approx(TwoPi<Real>::value));
    REQUIRE(disc.dy() == Approx(TwoPi<Real>::value / NY));
    REQUIRE(disc.lattice_vectors().size() == 2);
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[0],
                       (typename lattices::SquareDiscretization<Real>::Vector{disc.dx(), 0.})));
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[1],
                       (typename lattices::SquareDiscretization<Real>::Vector{0., disc.dy()})));

    for(std::size_t i = 0; i < NX; ++i) {
        for(std::size_t j = 0; j < NY; ++j) {
            const auto idx = disc.index(i, j);
            REQUIRE(disc.begin()[idx] == idx);
            REQUIRE(VEC_APPROX(disc[idx], (typename lattices::SquareDiscretization<Real>::Vector{
                                              -Pi<Real>::value + i * disc.dx(),
                                              -Pi<Real>::value + j * disc.dy()})));
            REQUIRE(disc(disc[idx]) == idx);
        }
    }
}

template <typename Real>
void test_neighbors()
{
    lattices::SquareDiscretization<Real> disc(NX, NY, 1., 1.);

    std::array<typename lattices::SquareDiscretization<Real>::Index, 4> neigh;

    neigh = disc.neighbors(disc.index(0, 0));
    REQUIRE(neigh[0] == disc.index(NX - 1, 0));
    REQUIRE(neigh[1] == disc.index(0, NY - 1));
    REQUIRE(neigh[2] == disc.index(1, 0));
    REQUIRE(neigh[3] == disc.index(0, 1));

    neigh = disc.neighbors(disc.index(0, NY - 1));
    REQUIRE(neigh[0] == disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[1] == disc.index(0, NY - 2));
    REQUIRE(neigh[2] == disc.index(1, NY - 1));
    REQUIRE(neigh[3] == disc.index(0, 0));

    neigh = disc.neighbors(disc.index(NX - 1, 0));
    REQUIRE(neigh[0] == disc.index(NX - 2, 0));
    REQUIRE(neigh[1] == disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[2] == disc.index(0, 0));
    REQUIRE(neigh[3] == disc.index(NX - 1, 1));

    neigh = disc.neighbors(disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[0] == disc.index(NX - 2, NY - 1));
    REQUIRE(neigh[1] == disc.index(NX - 1, NY - 2));
    REQUIRE(neigh[2] == disc.index(0, NY - 1));
    REQUIRE(neigh[3] == disc.index(NX - 1, 0));

    for(std::size_t j = 1; j < NY - 1; ++j) {
        neigh = disc.neighbors(disc.index(0, j));
        REQUIRE(neigh[0] == disc.index(NX - 1, j));
        REQUIRE(neigh[1] == disc.index(0, j - 1));
        REQUIRE(neigh[2] == disc.index(1, j));
        REQUIRE(neigh[3] == disc.index(0, j + 1));

        neigh = disc.neighbors(disc.index(NX - 1, j));
        REQUIRE(neigh[0] == disc.index(NX - 2, j));
        REQUIRE(neigh[1] == disc.index(NX - 1, j - 1));
        REQUIRE(neigh[2] == disc.index(0, j));
        REQUIRE(neigh[3] == disc.index(NX - 1, j + 1));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        neigh = disc.neighbors(disc.index(i, 0));
        REQUIRE(neigh[0] == disc.index(i - 1, 0));
        REQUIRE(neigh[1] == disc.index(i, NY - 1));
        REQUIRE(neigh[2] == disc.index(i + 1, 0));
        REQUIRE(neigh[3] == disc.index(i, 1));

        neigh = disc.neighbors(disc.index(i, NY - 1));
        REQUIRE(neigh[0] == disc.index(i - 1, NY - 1));
        REQUIRE(neigh[1] == disc.index(i, NY - 2));
        REQUIRE(neigh[2] == disc.index(i + 1, NY - 1));
        REQUIRE(neigh[3] == disc.index(i, 0));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        for(std::size_t j = 1; j < NX - 1; ++j) {
            neigh = disc.neighbors(disc.index(i, j));
            REQUIRE(neigh[0] == disc.index(i - 1, j));
            REQUIRE(neigh[1] == disc.index(i, j - 1));
            REQUIRE(neigh[2] == disc.index(i + 1, j));
            REQUIRE(neigh[3] == disc.index(i, j + 1));
        }
    }
}

template <typename Real>
void test_unique_neighbors()
{
    lattices::SquareDiscretization<Real> disc(NX, NY, 1., 1.);

    auto neigh = disc.unique_neighbors(disc.index(0, 0));
    REQUIRE(neigh[0] == disc.index(1, 0));
    REQUIRE(neigh[1] == disc.index(0, 1));

    neigh = disc.unique_neighbors(disc.index(0, NY - 1));
    REQUIRE(neigh[0] == disc.index(1, NY - 1));
    REQUIRE(neigh[1] == disc.index(0, 0));

    neigh = disc.unique_neighbors(disc.index(NX - 1, 0));
    REQUIRE(neigh[0] == disc.index(0, 0));
    REQUIRE(neigh[1] == disc.index(NX - 1, 1));

    neigh = disc.unique_neighbors(disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[0] == disc.index(0, NY - 1));
    REQUIRE(neigh[1] == disc.index(NX - 1, 0));

    for(std::size_t j = 1; j < NY - 1; ++j) {
        neigh = disc.unique_neighbors(disc.index(0, j));
        REQUIRE(neigh[0] == disc.index(1, j));
        REQUIRE(neigh[1] == disc.index(0, j + 1));

        neigh = disc.unique_neighbors(disc.index(NX - 1, j));
        REQUIRE(neigh[0] == disc.index(0, j));
        REQUIRE(neigh[1] == disc.index(NX - 1, j + 1));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        neigh = disc.unique_neighbors(disc.index(i, 0));
        REQUIRE(neigh[0] == disc.index(i + 1, 0));
        REQUIRE(neigh[1] == disc.index(i, 1));

        neigh = disc.unique_neighbors(disc.index(i, NY - 1));
        REQUIRE(neigh[0] == disc.index(i + 1, NY - 1));
        REQUIRE(neigh[1] == disc.index(i, 0));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        for(std::size_t j = 1; j < NX - 1; ++j) {
            neigh = disc.unique_neighbors(disc.index(i, j));
            REQUIRE(neigh[0] == disc.index(i + 1, j));
            REQUIRE(neigh[1] == disc.index(i, j + 1));
        }
    }
}

template <typename Real>
void test_closest()
{
    using Vector = typename lattices::SquareDiscretization<Real>::Vector;

    lattices::SquareDiscretization<Real> disc(NX, NY, 1., 1.);
    REQUIRE(disc.closest(Vector{0.49, 0.49}) == disc.index(0, 0));
    REQUIRE(disc.closest(Vector{0.51, 0.49}) == disc.index(1, 0));
    REQUIRE(disc.closest(Vector{0.49, 0.51}) == disc.index(0, 1));
    REQUIRE(disc.closest(Vector{0.51, 0.51}) == disc.index(1, 1));
    REQUIRE(disc.closest(Vector{-0.49, -0.49}) == disc.index(0, 0));
    REQUIRE(disc.closest(Vector{-0.51, -0.49}) == disc.index(NX - 1, 0));
    REQUIRE(disc.closest(Vector{-0.49, -0.51}) == disc.index(0, NY - 1));
    REQUIRE(disc.closest(Vector{-0.51, -0.51}) == disc.index(NX - 1, NY - 1));
}

TEST_CASE("initialization (real space)")
{
    test_initialization_real_space<float>();
    test_initialization_real_space<double>();
    test_initialization_real_space<long double>();
}

TEST_CASE("initialization (momentum space)")
{
    test_initialization_momentum_space<float>();
    test_initialization_momentum_space<double>();
    test_initialization_momentum_space<long double>();
}

TEST_CASE("closest site")
{
    test_closest<float>();
    test_closest<double>();
    test_closest<long double>();
}

TEST_CASE("neighbors")
{
    test_neighbors<float>();
    test_neighbors<double>();
    test_neighbors<long double>();
}

TEST_CASE("unique neighbors")
{
    test_unique_neighbors<float>();
    test_unique_neighbors<double>();
    test_unique_neighbors<long double>();
}
