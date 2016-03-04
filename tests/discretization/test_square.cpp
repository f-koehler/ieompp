#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/discretization/square.hpp"

TEST_CASE("square discretization (real space)", "")
{
    using Discretization = hubbard::discretization::SquareDiscretization<double>;
    const std::size_t nx = 10;
    const std::size_t ny = 10;
    Discretization disc(nx, ny, 1., 1.);

    SECTION("initialization")
    {
        REQUIRE(disc.indices.size() == nx * ny);
        REQUIRE(disc.sites.size() == nx);

        REQUIRE(disc.num_x == nx);
        REQUIRE(disc.num_y == ny);
        REQUIRE(disc.dx == 1.);
        REQUIRE(disc.dy == 1.);
        REQUIRE(disc.x_min == 0.);
        REQUIRE(disc.x_max == double(nx - 1));
        REQUIRE(disc.y_min == 0.);
        REQUIRE(disc.y_max == double(ny - 1));

        for(std::size_t i = 0; i < nx; ++i) {
            REQUIRE(disc.sites[i].size() == ny);
            for(std::size_t j = 0; j < ny; ++j) {
                REQUIRE(disc.indices[i * nx + j] == std::make_tuple(i, j));
                REQUIRE(disc[disc.indices[i * nx + j]]
                        == Discretization::Vector(double(i), double(j)));
            }
        }
    }

    SECTION("closest")
    {
        REQUIRE(disc.closest(Discretization::Vector(0.4, 3.6)) == std::make_tuple(0, 4));
        REQUIRE(disc.closest(Discretization::Vector(0.2, 7.8)) == std::make_tuple(0, 8));
    }

    SECTION("neighbours")
    {
        std::array<Discretization::Index, 4> neigh;
        
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

    SECTION("unique neighbours")
    {
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

    SECTION("index operator")
    {
        for(auto& idx : disc.indices)
        {
            REQUIRE(disc[idx]
                    == Discretization::Vector(double(std::get<0>(idx)), double(std::get<1>(idx))));
        }
    }
}

TEST_CASE("square discretization (Fourier space)", "")
{
    using Discretization = hubbard::discretization::SquareDiscretization<double>;
    const std::size_t nx = 10;
    const std::size_t ny = 10;
    Discretization disc(nx, ny);

    REQUIRE(disc.x_min == -hubbard::Pi<double>::value);
    REQUIRE(disc.x_max == hubbard::Pi<double>::value);
    REQUIRE(disc.y_min == -hubbard::Pi<double>::value);
    REQUIRE(disc.y_max == hubbard::Pi<double>::value);

    REQUIRE(disc.dx == hubbard::TwoPi<double>::value / nx);
    REQUIRE(disc.dy == hubbard::TwoPi<double>::value / ny);
}
