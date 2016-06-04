include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/catch/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(unit_tests unit_tests.cpp)
add_test(NAME unit_tests COMMAND unit_tests)
