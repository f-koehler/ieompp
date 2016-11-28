add_executable(
    lattices.linear
    test_linear.cpp
)
add_executable(
    lattices.square
    test_square.cpp
)

add_ieompp_test(lattices.linear)
add_ieompp_test(lattices.square)
