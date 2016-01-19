add_executable(fermion_cas fermion_cas.cpp)
add_executable(regexes regexes.cpp)
add_executable(solve solve.cpp)

install(TARGETS fermion_cas RUNTIME DESTINATION bin)
