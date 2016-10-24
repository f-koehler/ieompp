add_subdirectory(hubbard)

add_executable(integrate_rk4 integrate_rk4.cpp)
target_link_libraries(integrate_rk4 ${Boost_LIBRARIES})
enable_iwyu(integrate_rk4)

install(
    TARGETS integrate_rk4
    RUNTIME
    DESTINATION bin
    CONFIGURATIONS Release
)
