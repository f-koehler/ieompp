add_executable(kinetic_jump_1d kinetic_jump_1d.cpp)
target_link_libraries(kinetic_jump_1d  ${Boost_LIBRARIES})

add_executable(kinetic_n_1d kinetic_n_1d.cpp)
target_link_libraries(kinetic_n_1d  ${Boost_LIBRARIES})

add_executable(jump_1d jump_1d.cpp)
target_link_libraries(jump_1d  ${Boost_LIBRARIES})

add_executable(n_1d n_1d.cpp)
target_link_libraries(n_1d  ${Boost_LIBRARIES})

add_executable(commutators commutators.cpp)

install(
    TARGETS
        kinetic_jump_1d
        kinetic_n_1d
        kinetic_theory_1d
        jump_1d
        n_1d
        commutators
    RUNTIME
    DESTINATION bin/hubbard/real_space
    CONFIGURATIONS Release
)
