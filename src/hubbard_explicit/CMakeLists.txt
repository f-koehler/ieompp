add_executable(real_1d real_1d.cpp)
add_executable(commutators_momentum_space commutators_momentum_space.cpp)
add_executable(commutators_real_space commutators_real_space.cpp)

#depend_eigen(commutators_momentum_space)
#depend_eigen(commutators_real_space)

# target_link_libraries(momentum_linear ${Boost_LIBRARIES})
