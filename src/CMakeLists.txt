add_subdirectory(hubbard_explicit)

add_executable(integrate_rk4 integrate_rk4.cpp)
target_link_libraries(integrate_rk4 ${Boost_LIBRARIES})
enable_iwyu(integrate_rk4)

add_executable(integrate_rk4_eigen integrate_rk4_eigen.cpp)
target_link_libraries(integrate_rk4_eigen ${Boost_LIBRARIES})
enable_iwyu(integrate_rk4_eigen)

add_executable(integrate_rkf45 integrate_rkf45.cpp)
target_link_libraries(integrate_rkf45 ${Boost_LIBRARIES})
enable_iwyu(integrate_rkf45)

add_executable(integrate_rkck integrate_rkck.cpp)
target_link_libraries(integrate_rkck ${Boost_LIBRARIES})
enable_iwyu(integrate_rkck)

install(
    TARGETS integrate_rk4 integrate_rk4_eigen integrate_rkf45 integrate_rkck
    RUNTIME
    DESTINATION bin
    CONFIGURATIONS Release
)
