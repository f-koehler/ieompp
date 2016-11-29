add_executable(n_1d n_1d.cpp)
target_link_libraries(n_1d  ${Boost_LIBRARIES})

add_executable(commutators commutators.cpp)

install(
    TARGETS
        n_1d
        commutators
    RUNTIME
    DESTINATION bin/hubbard/real_space
    CONFIGURATIONS Release
)
