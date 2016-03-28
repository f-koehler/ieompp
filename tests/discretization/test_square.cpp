#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/discretization/square.hpp"
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

    REQUIRE(disc.end()-disc.begin() == N);
    REQUIRE(disc.cend()-disc.cbegin() == N);

    for(std::size_t i = 0; i < NX; ++i) {
        for(std::size_t j = 0; j < NY; ++j) {
            const auto idx = i*NX+j;
            REQUIRE(disc.begin()[idx] == idx);
            REQUIRE(VEC_APPROX(disc[idx], typename SquareDiscretization<Real>::Vector(Real(i), Real(j))));
            REQUIRE(disc[disc[idx]] == idx);
        }
    }
}

/* template <typename Real> */
/* void test_initialization_momentum_space() */
/* { */
/*     SquareDiscretization<Real> disc(nx, ny); */

/*     auto pi = ieompp::Pi<Real>::value; */

/*     REQUIRE(disc.x_min == Approx(-pi)); */
/*     REQUIRE(disc.x_max == Approx(pi)); */
/*     REQUIRE(disc.y_min == Approx(-pi)); */
/*     REQUIRE(disc.y_max == Approx(pi)); */

/*     REQUIRE(disc.dx == Approx(ieompp::TwoPi<Real>::value / nx)); */
/*     REQUIRE(disc.dy == Approx(ieompp::TwoPi<Real>::value / ny)); */

/*     REQUIRE(VEC_APPROX(disc.sites.front().front(), typename SquareDiscretization<Real>::Vector(-pi, -pi))); */
/*     REQUIRE(VEC_APPROX(disc.sites.front().back(), typename SquareDiscretization<Real>::Vector(-pi, pi - disc.dy))); */
/*     REQUIRE(VEC_APPROX(disc.sites.back().front(), typename SquareDiscretization<Real>::Vector(pi - disc.dx, -pi))); */
/*     REQUIRE(VEC_APPROX(disc.sites.back().back(), typename SquareDiscretization<Real>::Vector(pi - disc.dx, pi - disc.dy))); */
/* } */

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

TEST_CASE("initialization (real space)", "")
{
    test_initialization_real_space<float>();
    test_initialization_real_space<double>();
    test_initialization_real_space<long double>();
}

/* TEST_CASE("initialization (momentum space)", "") */
/* { */
/*     test_initialization_momentum_space<float>(); */
/*     test_initialization_momentum_space<double>(); */
/*     test_initialization_momentum_space<long double>(); */
/* } */

/* TEST_CASE("closest site", "") */
/* { */
/*     test_closest_site<float>(); */
/*     test_closest_site<double>(); */
/*     test_closest_site<long double>(); */
/* } */

TEST_CASE("neighbours", "")
{
    test_neighbours<float>();
    test_neighbours<double>();
    test_neighbours<long double>();
}

TEST_CASE("unique neighbours", "")
{
    test_unique_neighbours<float>();
    test_unique_neighbours<double>();
    test_unique_neighbours<long double>();
}
