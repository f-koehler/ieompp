add_executable(hubbard_commutators_momentum_space hubbard_commutators_momentum_space.cpp)
add_executable(hubbard_commutators_real_space hubbard_commutators_real_space.cpp)

add_executable(hubbard_trivial hubbard_trivial.cpp)

add_executable(hubbard_momentum_1d_rk4 hubbard_momentum_1d_rk4.cpp)
target_link_libraries(hubbard_momentum_1d_rk4  ${Boost_LIBRARIES})
enable_iwyu(hubbard_momentum_1d_rk4)

add_executable(hubbard_real_1d_rk4 hubbard_real_1d_rk4.cpp)
target_link_libraries(hubbard_real_1d_rk4  ${Boost_LIBRARIES})
enable_iwyu(hubbard_real_1d_rk4)

add_executable(hubbard_real_1d_kinetic_rk4 hubbard_real_1d_kinetic_rk4.cpp)
target_link_libraries(hubbard_real_1d_kinetic_rk4  ${Boost_LIBRARIES})
enable_iwyu(hubbard_real_1d_kinetic_rk4)

add_executable(hubbard_real_1d_kinetic_theory hubbard_real_1d_kinetic_theory.cpp)
target_link_libraries(hubbard_real_1d_kinetic_theory ${Boost_LIBRARIES})
enable_iwyu(hubbard_real_1d_kinetic_theory)

install(
    TARGETS
        hubbard_commutators_momentum_space
        hubbard_commutators_real_space
        hubbard_momentum_1d_rk4
        hubbard_real_1d_kinetic_rk4
        hubbard_real_1d_kinetic_theory
    RUNTIME
    DESTINATION bin
    CONFIGURATIONS Release
)
