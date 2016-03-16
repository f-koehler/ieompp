#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/discretization/square.hpp"
using namespace hubbard::discretization;

#define VEC_APPROX(a, b) ((Approx(a[0]) == b[0]) && (Approx(a[1]) == b[1]))

const std::size_t nx = 10;
const std::size_t ny = 10;

template <typename Real>
void test_initialization_real_space()
{
    SquareDiscretization<Real> disc(nx, ny, 1., 1.);

    REQUIRE(disc.indices.size() == nx * ny);
    REQUIRE(disc.sites.size() == nx);

    REQUIRE(disc.num_x == nx);
    REQUIRE(disc.num_y == ny);
    REQUIRE(disc.dx == 1.);
    REQUIRE(disc.dy == 1.);
    REQUIRE(disc.x_min == 0.);
    REQUIRE(disc.x_max == Real(nx - 1));
    REQUIRE(disc.y_min == 0.);
    REQUIRE(disc.y_max == Real(ny - 1));

    for(std::size_t i = 0; i < nx; ++i) {
        REQUIRE(disc.sites[i].size() == ny);
        for(std::size_t j = 0; j < ny; ++j) {
            REQUIRE(disc.indices[i * nx + j] == std::make_tuple(i, j));
            REQUIRE(VEC_APPROX(disc[disc.indices[i * nx + j]], typename SquareDiscretization<Real>::Vector(Real(i), Real(j))));
        }
    }
}

template <typename Real>
void test_initialization_momentum_space()
{
    SquareDiscretization<Real> disc(nx, ny);

    auto pi = hubbard::Pi<Real>::value;

    REQUIRE(disc.x_min == Approx(-pi));
    REQUIRE(disc.x_max == Approx(pi));
    REQUIRE(disc.y_min == Approx(-pi));
    REQUIRE(disc.y_max == Approx(pi));

    REQUIRE(disc.dx == Approx(hubbard::TwoPi<Real>::value / nx));
    REQUIRE(disc.dy == Approx(hubbard::TwoPi<Real>::value / ny));

    REQUIRE(VEC_APPROX(disc.sites.front().front(), typename SquareDiscretization<Real>::Vector(-pi, -pi)));
    REQUIRE(VEC_APPROX(disc.sites.front().back(), typename SquareDiscretization<Real>::Vector(-pi, pi - disc.dy)));
    REQUIRE(VEC_APPROX(disc.sites.back().front(), typename SquareDiscretization<Real>::Vector(pi - disc.dx, -pi)));
    REQUIRE(VEC_APPROX(disc.sites.back().back(), typename SquareDiscretization<Real>::Vector(pi - disc.dx, pi - disc.dy)));
}

template <typename Real>
void test_closest_site()
{
    SquareDiscretization<Real> disc(nx, ny, 1., 1.);

    REQUIRE(disc.closest(typename SquareDiscretization<Real>::Vector(0.4, 3.6)) == std::make_tuple(0, 4));
    REQUIRE(disc.closest(typename SquareDiscretization<Real>::Vector(0.2, 7.8)) == std::make_tuple(0, 8));
}

template <typename Real>
void test_neighbours()
{
    SquareDiscretization<Real> disc(nx, ny, 1., 1.);

    std::array<typename SquareDiscretization<Real>::Index, 4> neigh;

    neigh = disc.neighbours(std::make_tuple(0, 0));
    REQUIRE(neigh[0] == std::make_tuple(nx - 1, 0));
    REQUIRE(neigh[1] == std::make_tuple(0, ny - 1));
    REQUIRE(neigh[2] == std::make_tuple(1, 0));
    REQUIRE(neigh[3] == std::make_tuple(0, 1));

    neigh = disc.neighbours(std::make_tuple(0, ny - 1));
    REQUIRE(neigh[0] == std::make_tuple(nx - 1, ny - 1));
    REQUIRE(neigh[1] == std::make_tuple(0, ny - 2));
    REQUIRE(neigh[2] == std::make_tuple(1, ny - 1));
    REQUIRE(neigh[3] == std::make_tuple(0, 0));

    neigh = disc.neighbours(std::make_tuple(nx - 1, 0));
    REQUIRE(neigh[0] == std::make_tuple(nx - 2, 0));
    REQUIRE(neigh[1] == std::make_tuple(nx - 1, ny - 1));
    REQUIRE(neigh[2] == std::make_tuple(0, 0));
    REQUIRE(neigh[3] == std::make_tuple(nx - 1, 1));

    neigh = disc.neighbours(std::make_tuple(nx - 1, ny - 1));
    REQUIRE(neigh[0] == std::make_tuple(nx - 2, ny - 1));
    REQUIRE(neigh[1] == std::make_tuple(nx - 1, ny - 2));
    REQUIRE(neigh[2] == std::make_tuple(0, ny - 1));
    REQUIRE(neigh[3] == std::make_tuple(nx - 1, 0));

    for(std::size_t j = 1; j < ny - 1; ++j) {
        neigh = disc.neighbours(std::make_tuple(0, j));
        REQUIRE(neigh[0] == std::make_tuple(nx - 1, j));
        REQUIRE(neigh[1] == std::make_tuple(0, j - 1));
        REQUIRE(neigh[2] == std::make_tuple(1, j));
        REQUIRE(neigh[3] == std::make_tuple(0, j + 1));

        neigh = disc.neighbours(std::make_tuple(nx - 1, j));
        REQUIRE(neigh[0] == std::make_tuple(nx - 2, j));
        REQUIRE(neigh[1] == std::make_tuple(nx - 1, j - 1));
        REQUIRE(neigh[2] == std::make_tuple(0, j));
        REQUIRE(neigh[3] == std::make_tuple(nx - 1, j + 1));
    }

    for(std::size_t i = 1; i < nx - 1; ++i) {
        neigh = disc.neighbours(std::make_tuple(i, 0));
        REQUIRE(neigh[0] == std::make_tuple(i - 1, 0));
        REQUIRE(neigh[1] == std::make_tuple(i, ny - 1));
        REQUIRE(neigh[2] == std::make_tuple(i + 1, 0));
        REQUIRE(neigh[3] == std::make_tuple(i, 1));

        neigh = disc.neighbours(std::make_tuple(i, ny - 1));
        REQUIRE(neigh[0] == std::make_tuple(i - 1, ny - 1));
        REQUIRE(neigh[1] == std::make_tuple(i, ny - 2));
        REQUIRE(neigh[2] == std::make_tuple(i + 1, ny - 1));
        REQUIRE(neigh[3] == std::make_tuple(i, 0));
    }

    for(std::size_t i = 1; i < nx - 1; ++i) {
        for(std::size_t j = 1; j < nx - 1; ++j) {
            neigh = disc.neighbours(std::make_tuple(i, j));
            REQUIRE(neigh[0] == std::make_tuple(i - 1, j));
            REQUIRE(neigh[1] == std::make_tuple(i, j - 1));
            REQUIRE(neigh[2] == std::make_tuple(i + 1, j));
            REQUIRE(neigh[3] == std::make_tuple(i, j + 1));
        }
    }
}

template <typename Real>
void test_unique_neighbours()
{
    SquareDiscretization<Real> disc(nx, ny, 1., 1.);

    auto neigh = disc.unique_neighbours(std::make_tuple(0, 0));
    REQUIRE(neigh[0] == std::make_tuple(1, 0));
    REQUIRE(neigh[1] == std::make_tuple(0, 1));

    neigh = disc.unique_neighbours(std::make_tuple(0, ny - 1));
    REQUIRE(neigh[0] == std::make_tuple(1, ny - 1));
    REQUIRE(neigh[1] == std::make_tuple(0, 0));

    neigh = disc.unique_neighbours(std::make_tuple(nx - 1, 0));
    REQUIRE(neigh[0] == std::make_tuple(0, 0));
    REQUIRE(neigh[1] == std::make_tuple(nx - 1, 1));

    neigh = disc.unique_neighbours(std::make_tuple(nx - 1, ny - 1));
    REQUIRE(neigh[0] == std::make_tuple(0, ny - 1));
    REQUIRE(neigh[1] == std::make_tuple(nx - 1, 0));

    for(std::size_t j = 1; j < ny - 1; ++j) {
        neigh = disc.unique_neighbours(std::make_tuple(0, j));
        REQUIRE(neigh[0] == std::make_tuple(1, j));
        REQUIRE(neigh[1] == std::make_tuple(0, j + 1));

        neigh = disc.unique_neighbours(std::make_tuple(nx - 1, j));
        REQUIRE(neigh[0] == std::make_tuple(0, j));
        REQUIRE(neigh[1] == std::make_tuple(nx - 1, j + 1));
    }

    for(std::size_t i = 1; i < nx - 1; ++i) {
        neigh = disc.unique_neighbours(std::make_tuple(i, 0));
        REQUIRE(neigh[0] == std::make_tuple(i + 1, 0));
        REQUIRE(neigh[1] == std::make_tuple(i, 1));

        neigh = disc.unique_neighbours(std::make_tuple(i, ny - 1));
        REQUIRE(neigh[0] == std::make_tuple(i + 1, ny - 1));
        REQUIRE(neigh[1] == std::make_tuple(i, 0));
    }

    for(std::size_t i = 1; i < nx - 1; ++i) {
        for(std::size_t j = 1; j < nx - 1; ++j) {
            neigh = disc.unique_neighbours(std::make_tuple(i, j));
            REQUIRE(neigh[0] == std::make_tuple(i + 1, j));
            REQUIRE(neigh[1] == std::make_tuple(i, j + 1));
        }
    }
}

template <typename Real>
void test_indexing()
{
    SquareDiscretization<Real> disc(nx, ny, 1., 1.);

    SECTION("index operator")
    {
        for(auto& idx : disc.indices) {
            REQUIRE(VEC_APPROX(disc[idx], typename SquareDiscretization<Real>::Vector(
                                              Real(std::get<0>(idx)), Real(std::get<1>(idx)))));
        }
    }

    SECTION("index operator for vector type")
    {
        for(auto& idx : disc.indices) REQUIRE(idx == disc[disc[idx]]);
    }
}

TEST_CASE("initialization (real space)", "")
{
    test_initialization_real_space<float>();
    test_initialization_real_space<double>();
    test_initialization_real_space<long double>();
}

TEST_CASE("initialization (momentum space)", "")
{
    test_initialization_momentum_space<float>();
    test_initialization_momentum_space<double>();
    test_initialization_momentum_space<long double>();
}

TEST_CASE("closest site", "")
{
    test_closest_site<float>();
    test_closest_site<double>();
    test_closest_site<long double>();
}

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

TEST_CASE("indexing", "")
{
    test_indexing<float>();
    test_indexing<double>();
    test_indexing<long double>();
}
