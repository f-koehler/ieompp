add_executable(real_1d real_1d.cpp)
add_executable(commutators_momentum_space commutators_momentum_space.cpp)
add_executable(commutators_real_space commutators_real_space.cpp)

depend_eigen(real_1d)
target_link_libraries(real_1d ${Boost_LIBRARIES})
