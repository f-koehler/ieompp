include_directories(${CMAKE_SOURCE_DIR}/external/quicli)

add_executable(fermion_cas fermion_cas.cpp)
add_executable(hamiltonian hamiltonian.cpp)
add_executable(regexes regexes.cpp)
add_executable(solve solve.cpp)
add_executable(print print.cpp)

if(${ieompp_HAS_BOOST})
    target_link_libraries(solve "${Boost_LIBRARIES}")
endif()

if(ieompp_ENABLE_IWYU)
    add_iwyu_analysis(fermion_cas.iwyu fermion_cas.cpp)
    add_iwyu_analysis(regexes.iwyu regexes.cpp)
    add_iwyu_analysis(solve.iwyu solve.cpp)
endif()

install(TARGETS fermion_cas RUNTIME DESTINATION bin)
