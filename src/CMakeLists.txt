include_directories(${CMAKE_SOURCE_DIR}/external/quicli)

add_executable(fermion_cas fermion_cas.cpp)
add_executable(regexes regexes.cpp)
add_executable(solve solve.cpp)

if(hubbard_ENABLE_IWYU)
    add_iwyu_analysis(fermion_cas.iwyu fermion_cas.cpp)
    add_iwyu_analysis(regexes.iwyu regexes.cpp)
    add_iwyu_analysis(solve.iwyu solve.cpp)
endif()

install(TARGETS fermion_cas RUNTIME DESTINATION bin)
