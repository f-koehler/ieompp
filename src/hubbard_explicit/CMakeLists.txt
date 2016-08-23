add_executable(basis_momentum_linear basis_momentum_linear.cpp)
add_executable(basis_real_linear basis_real_linear.cpp)
add_executable(momentum_linear momentum_linear.cpp)
add_executable(real_linear real_linear.cpp)

target_link_libraries(basis_momentum_linear ${Boost_LIBRARIES})
target_link_libraries(basis_real_linear ${Boost_LIBRARIES})
target_link_libraries(momentum_linear ${Boost_LIBRARIES})
target_link_libraries(real_linear ${Boost_LIBRARIES})
