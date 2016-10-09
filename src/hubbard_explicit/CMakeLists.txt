add_executable(hubbard_commutators_momentum_space hubbard_commutators_momentum_space.cpp)
add_executable(hubbard_commutators_real_space hubbard_commutators_real_space.cpp)

add_executable(hubbard_interaction_matrix_1d_real hubbard_interaction_matrix_1d_real.cpp)
target_link_libraries(hubbard_interaction_matrix_1d_real ${Boost_LIBRARIES})
enable_iwyu(hubbard_interaction_matrix_1d_real)

add_executable(hubbard_kinetic_matrix_1d_real hubbard_kinetic_matrix_1d_real.cpp)
target_link_libraries(hubbard_kinetic_matrix_1d_real ${Boost_LIBRARIES})
enable_iwyu(hubbard_kinetic_matrix_1d_real)

add_executable(hubbard_kinetic_matrix_1d_real_1 hubbard_kinetic_matrix_1d_real_1.cpp)
target_link_libraries(hubbard_kinetic_matrix_1d_real_1 ${Boost_LIBRARIES})
enable_iwyu(hubbard_kinetic_matrix_1d_real_1)

add_executable(hubbard_matrix_1d_real hubbard_matrix_1d_real.cpp)
target_link_libraries(hubbard_matrix_1d_real ${Boost_LIBRARIES})
enable_iwyu(hubbard_matrix_1d_real)

add_executable(hubbard_1d_real hubbard_1d_real.cpp)
target_link_libraries(hubbard_1d_real ${Boost_LIBRARIES})
enable_iwyu(hubbard_1d_real)

install(
    TARGETS
        hubbard_commutators_momentum_space
        hubbard_commutators_real_space
        hubbard_interaction_matrix_1d_real
        hubbard_kinetic_matrix_1d_real
        hubbard_kinetic_matrix_1d_real_1
        hubbard_matrix_1d_real
        hubbard_1d_real
    RUNTIME
    DESTINATION bin
    CONFIGURATIONS Release
)
