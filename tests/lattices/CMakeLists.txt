add_executable(
    lattices.periodic_chain
    test_periodic_chain.cpp
)
add_executable(
    lattices.periodic_square_lattice
    test_periodic_square_lattice.cpp
)

add_ieompp_test(lattices.periodic_chain)
add_ieompp_test(lattices.periodic_square_lattice)
