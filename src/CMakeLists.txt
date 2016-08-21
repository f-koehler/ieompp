# add_executable(commutators commutators.cpp)
# add_executable(commutators_real_space_1d commutators_real_space_1d.cpp)
# add_executable(commutators_real_space_2d commutators_real_space_2d.cpp)
# add_executable(hubbard_exp_real_1d hubbard_exp_real_1d.cpp)
# add_executable(hubbard_exp_momentum_1d hubbard_exp_momentum_1d.cpp)
# add_executable(inner_product inner_product.cpp)

add_executable(main main.cpp)
add_executable(basis_momentum_linear basis_momentum_linear.cpp)
add_executable(momentum_linear momentum_linear.cpp)

target_link_libraries(basis_momentum_linear ${Boost_LIBRARIES})
target_link_libraries(momentum_linear ${Boost_LIBRARIES})
