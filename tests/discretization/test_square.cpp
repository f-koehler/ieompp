#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/discretization/square.hpp"
using namespace ieompp;
using namespace ieompp::discretization;

#include <iostream>
using namespace std;

#define VEC_APPROX(a, b) ((Approx(a(0)) == b(0)) && (Approx(a(1)) == b(1)))

const std::size_t NX = 10;
const std::size_t NY = 10;
const std::size_t N  = NX * NY;

template <typename Real>
void test_initialization_real_space()
{
    SquareDiscretization<Real> disc(NX, NY, 1., 1.);

    REQUIRE(disc.end() - disc.begin() == N);
    REQUIRE(disc.cend() - disc.cbegin() == N);

    REQUIRE(disc.num() == N);
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
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[0], typename SquareDiscretization<Real>::Vector(1., 0.)));
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[1], typename SquareDiscretization<Real>::Vector(0., 1.)));

    for(std::size_t i = 0; i < NX; ++i) {
        for(std::size_t j = 0; j < NY; ++j) {
            const auto idx = disc.index(i, j);
            REQUIRE(disc.begin()[idx] == idx);
            REQUIRE(VEC_APPROX(disc[idx], typename SquareDiscretization<Real>::Vector(Real(i), Real(j))));
            REQUIRE(disc(disc[idx]) == idx);
        }
    }
}

template <typename Real>
void test_initialization_momentum_space()
{
    SquareDiscretization<Real> disc(NX, NY);

    REQUIRE(disc.num() == N);
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
                       typename SquareDiscretization<Real>::Vector(disc.dx(), 0.)));
    REQUIRE(VEC_APPROX(disc.lattice_vectors()[1],
                       typename SquareDiscretization<Real>::Vector(0., disc.dy())));

    for(std::size_t i = 0; i < NX; ++i) {
        for(std::size_t j = 0; j < NY; ++j) {
            const auto idx = disc.index(i, j);
            REQUIRE(disc.begin()[idx] == idx);
            REQUIRE(VEC_APPROX(disc[idx], typename SquareDiscretization<Real>::Vector(
                                              -Pi<Real>::value + i * disc.dx(),
                                              -Pi<Real>::value + j * disc.dy())));
            REQUIRE(disc(disc[idx]) == idx);
        }
    }
}

template <typename Real>
void test_neighbours()
{
    SquareDiscretization<Real> disc(NX, NY, 1., 1.);

    std::array<typename SquareDiscretization<Real>::Index, 4> neigh;

    neigh = disc.neighbours(disc.index(0, 0));
    REQUIRE(neigh[0] == disc.index(NX - 1, 0));
    REQUIRE(neigh[1] == disc.index(0, NY - 1));
    REQUIRE(neigh[2] == disc.index(1, 0));
    REQUIRE(neigh[3] == disc.index(0, 1));

    neigh = disc.neighbours(disc.index(0, NY - 1));
    REQUIRE(neigh[0] == disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[1] == disc.index(0, NY - 2));
    REQUIRE(neigh[2] == disc.index(1, NY - 1));
    REQUIRE(neigh[3] == disc.index(0, 0));

    neigh = disc.neighbours(disc.index(NX - 1, 0));
    REQUIRE(neigh[0] == disc.index(NX - 2, 0));
    REQUIRE(neigh[1] == disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[2] == disc.index(0, 0));
    REQUIRE(neigh[3] == disc.index(NX - 1, 1));

    neigh = disc.neighbours(disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[0] == disc.index(NX - 2, NY - 1));
    REQUIRE(neigh[1] == disc.index(NX - 1, NY - 2));
    REQUIRE(neigh[2] == disc.index(0, NY - 1));
    REQUIRE(neigh[3] == disc.index(NX - 1, 0));

    for(std::size_t j = 1; j < NY - 1; ++j) {
        neigh = disc.neighbours(disc.index(0, j));
        REQUIRE(neigh[0] == disc.index(NX - 1, j));
        REQUIRE(neigh[1] == disc.index(0, j - 1));
        REQUIRE(neigh[2] == disc.index(1, j));
        REQUIRE(neigh[3] == disc.index(0, j + 1));

        neigh = disc.neighbours(disc.index(NX - 1, j));
        REQUIRE(neigh[0] == disc.index(NX - 2, j));
        REQUIRE(neigh[1] == disc.index(NX - 1, j - 1));
        REQUIRE(neigh[2] == disc.index(0, j));
        REQUIRE(neigh[3] == disc.index(NX - 1, j + 1));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        neigh = disc.neighbours(disc.index(i, 0));
        REQUIRE(neigh[0] == disc.index(i - 1, 0));
        REQUIRE(neigh[1] == disc.index(i, NY - 1));
        REQUIRE(neigh[2] == disc.index(i + 1, 0));
        REQUIRE(neigh[3] == disc.index(i, 1));

        neigh = disc.neighbours(disc.index(i, NY - 1));
        REQUIRE(neigh[0] == disc.index(i - 1, NY - 1));
        REQUIRE(neigh[1] == disc.index(i, NY - 2));
        REQUIRE(neigh[2] == disc.index(i + 1, NY - 1));
        REQUIRE(neigh[3] == disc.index(i, 0));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        for(std::size_t j = 1; j < NX - 1; ++j) {
            neigh = disc.neighbours(disc.index(i, j));
            REQUIRE(neigh[0] == disc.index(i - 1, j));
            REQUIRE(neigh[1] == disc.index(i, j - 1));
            REQUIRE(neigh[2] == disc.index(i + 1, j));
            REQUIRE(neigh[3] == disc.index(i, j + 1));
        }
    }
}

template <typename Real>
void test_unique_neighbours()
{
    SquareDiscretization<Real> disc(NX, NY, 1., 1.);

    auto neigh = disc.unique_neighbours(disc.index(0, 0));
    REQUIRE(neigh[0] == disc.index(1, 0));
    REQUIRE(neigh[1] == disc.index(0, 1));

    neigh = disc.unique_neighbours(disc.index(0, NY - 1));
    REQUIRE(neigh[0] == disc.index(1, NY - 1));
    REQUIRE(neigh[1] == disc.index(0, 0));

    neigh = disc.unique_neighbours(disc.index(NX - 1, 0));
    REQUIRE(neigh[0] == disc.index(0, 0));
    REQUIRE(neigh[1] == disc.index(NX - 1, 1));

    neigh = disc.unique_neighbours(disc.index(NX - 1, NY - 1));
    REQUIRE(neigh[0] == disc.index(0, NY - 1));
    REQUIRE(neigh[1] == disc.index(NX - 1, 0));

    for(std::size_t j = 1; j < NY - 1; ++j) {
        neigh = disc.unique_neighbours(disc.index(0, j));
        REQUIRE(neigh[0] == disc.index(1, j));
        REQUIRE(neigh[1] == disc.index(0, j + 1));

        neigh = disc.unique_neighbours(disc.index(NX - 1, j));
        REQUIRE(neigh[0] == disc.index(0, j));
        REQUIRE(neigh[1] == disc.index(NX - 1, j + 1));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        neigh = disc.unique_neighbours(disc.index(i, 0));
        REQUIRE(neigh[0] == disc.index(i + 1, 0));
        REQUIRE(neigh[1] == disc.index(i, 1));

        neigh = disc.unique_neighbours(disc.index(i, NY - 1));
        REQUIRE(neigh[0] == disc.index(i + 1, NY - 1));
        REQUIRE(neigh[1] == disc.index(i, 0));
    }

    for(std::size_t i = 1; i < NX - 1; ++i) {
        for(std::size_t j = 1; j < NX - 1; ++j) {
            neigh = disc.unique_neighbours(disc.index(i, j));
            REQUIRE(neigh[0] == disc.index(i + 1, j));
            REQUIRE(neigh[1] == disc.index(i, j + 1));
        }
    }
}

template <typename Real>
void test_closest()
{
    using Vector = typename SquareDiscretization<Real>::Vector;

    SquareDiscretization<Real> disc(NX, NY, 1., 1.);
    REQUIRE(disc.closest(Vector(0.49, 0.49)) == disc.index(0, 0));
    REQUIRE(disc.closest(Vector(0.51, 0.49)) == disc.index(1, 0));
    REQUIRE(disc.closest(Vector(0.49, 0.51)) == disc.index(0, 1));
    REQUIRE(disc.closest(Vector(0.51, 0.51)) == disc.index(1, 1));
    REQUIRE(disc.closest(Vector(-0.49, -0.49)) == disc.index(0, 0));
    REQUIRE(disc.closest(Vector(-0.51, -0.49)) == disc.index(NX - 1, 0));
    REQUIRE(disc.closest(Vector(-0.49, -0.51)) == disc.index(0, NY - 1));
    REQUIRE(disc.closest(Vector(-0.51, -0.51)) == disc.index(NX - 1, NY - 1));
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

TEST_CASE("neighbours")
{
    test_neighbours<float>();
    test_neighbours<double>();
    test_neighbours<long double>();
}

TEST_CASE("unique neighbours")
{
    test_unique_neighbours<float>();
    test_unique_neighbours<double>();
    test_unique_neighbours<long double>();
}
