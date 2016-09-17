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

# fetch Eigen
include(cmake/get-eigen.cmake)
get_eigen(${CMAKE_SOURCE_DIR}/external/eigen)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/eigen)

find_package(Boost REQUIRED COMPONENTS program_options unit_test_framework)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# enable OpenMP if possible
find_package(OpenMP)
if(${OPENMP_FOUND})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(src)

# option to enable unit testing
option(ieompp_ENABLE_TESTS "Enable unit tests" ON)
if(${ieompp_ENABLE_TESTS})
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        option(ieompp_ENABLE_CLANG_COVERAGE "Enable Clang coverage reports" OFF)
    endif()
    enable_testing()
    add_subdirectory(tests)
endif()
