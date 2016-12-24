add_executable(hubbard_commutators_momentum_space hubbard_commutators_momentum_space.cpp)
add_executable(hubbard_commutators_real_space hubbard_commutators_real_space.cpp)

add_executable(hubbard_trivial hubbard_trivial.cpp)

add_executable(hubbard_momentum_1d_rk4 hubbard_momentum_1d_rk4.cpp)
target_link_libraries(hubbard_momentum_1d_rk4 ${Boost_LIBRARIES})

add_executable(hubbard_real_1d_rk4 hubbard_real_1d_rk4.cpp)
target_link_libraries(hubbard_real_1d_rk4 ${Boost_LIBRARIES})

add_executable(hubbard_real_1d_rk4_jump hubbard_real_1d_rk4_jump.cpp)
target_link_libraries(hubbard_real_1d_rk4_jump ${Boost_LIBRARIES})

add_executable(hubbard_real_1d_rk4_jump_no hubbard_real_1d_rk4_jump_no.cpp)
target_link_libraries(hubbard_real_1d_rk4_jump_no ${Boost_LIBRARIES})

add_executable(hubbard_real_1d_kinetic_rk4 hubbard_real_1d_kinetic_rk4.cpp)
target_link_libraries(hubbard_real_1d_kinetic_rk4 ${Boost_LIBRARIES})

add_executable(hubbard_real_1d_kinetic_theory hubbard_real_1d_kinetic_theory.cpp)
target_link_libraries(hubbard_real_1d_kinetic_theory ${Boost_LIBRARIES})

add_executable(hubbard_dispersion_1d hubbard_dispersion_1d.cpp)

add_executable(hubbard_real_2d_rk4 hubbard_real_2d_rk4.cpp)
target_link_libraries(hubbard_real_2d_rk4 ${Boost_LIBRARIES})

install(
    TARGETS
        hubbard_commutators_momentum_space
        hubbard_commutators_real_space
        hubbard_momentum_1d_rk4
        hubbard_real_1d_kinetic_rk4
        hubbard_real_1d_kinetic_theory
        hubbard_real_1d_rk4
        hubbard_real_1d_rk4_jump
        hubbard_real_2d_rk4
    RUNTIME
    DESTINATION bin
    CONFIGURATIONS Release
)
