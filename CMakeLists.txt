cmake_minimum_required(VERSION 3.0)
project(ieompp)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# enable C++14 standard
include(cmake/cpp14.cmake)

# enable warnings
include(cmake/warnings.cmake)

# produce native binaries
include(cmake/native.cmake)

# create version.hpp
# include(cmake/version-script.cmake)
# run_version_script(${CMAKE_SOURCE_DIR}/tools/configure.py ${CMAKE_SOURCE_DIR}/ieompp/include/version.hpp)

# fetch Eigen as a tarball
include(cmake/get-eigen.cmake)
get_eigen(${CMAKE_SOURCE_DIR}/external/eigen)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/eigen)

add_executable(main main.cpp)
add_executable(example example.cpp)
add_executable(commutators commutators.cpp)
add_executable(commutators_real_space_1d commutators_real_space_1d.cpp)
add_executable(commutators_real_space_2d commutators_real_space_2d.cpp)

# option to enable unit testing
option(ieompp_ENABLE_TESTS "Enable unit tests" ON)
if(${ieompp_ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
