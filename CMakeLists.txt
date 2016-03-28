cmake_minimum_required(VERSION 3.0)
project(ieompp)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include(git.cmake)
include(cmake/speed.cmake)
include(cmake/warnings.cmake)

# hack to keep include/ieompp/config.hpp up-to-date
add_custom_target(
    configure
    ALL
    COMMAND python ${CMAKE_SOURCE_DIR}/tools/configure.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_command(
    COMMAND ${CMAKE_COMMAND} -E echo
    OUTPUT ${CMAKE_SOURCE_DIR}/ieompp/include/config.hpp
)

include(ExternalProject)
ExternalProject_Add(
    Eigen
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/eigen
    URL https://bitbucket.org/eigen/eigen/get/3.2.8.tar.gz
    URL_HASH SHA512=be52b81de726938e81ad4d2ef6d65e8d3ee9dc70038f22a4fd0b5df98bd2d4a4d1bc5f78e6580e81abaff389b8003f9239b0e1476f93ee9236c946ae215896b7
    CONFIGURE_COMMAND ":"
    CMAKE_COMMAND     ":"
    BUILD_COMMAND     ":"
    INSTALL_COMMAND   ":"
)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/eigen)

find_package(Boost)
if(${Boost_FOUND})
    set(ieompp_HAS_BOOST True)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    add_definitions(-DIEOMPP_HAS_BOOST)
endif()

option(ieompp_ENABLE_BENCHMARKS "Option to enable benchmarks" OFF)
option(ieompp_ENABLE_COVERAGE   "Option to enable code coverage measurement" OFF)
option(ieompp_ENABLE_COVERALLS  "Option to enable coverage report to coveralls.io" OFF)
option(ieompp_ENABLE_DOC        "Option to enable documentation" OFF)
option(ieompp_ENABLE_IWYU       "Option to enable #include analysis with include-what-you-use" OFF)
option(ieompp_ENABLE_OPENMP     "Option to enable OpenMP" OFF)
option(ieompp_ENABLE_TESTS      "Option to enable unittests" ON)

if(${ieompp_ENABLE_OPENMP})
    find_package(OpenMP REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if(${ieompp_ENABLE_IWYU})
    include(${CMAKE_SOURCE_DIR}/cmake/iwyu.cmake) 
endif()

if(${ieompp_ENABLE_TESTS})
    if(${ieompp_ENABLE_COVERAGE})
        include(cmake/coverage.cmake)
    endif()
    enable_testing()
    git_submodule(external/catch)
    add_subdirectory(tests)
endif()

if(${ieompp_ENABLE_BENCHMARKS})
    git_submodule(external/benchmark)
    add_subdirectory(benchmarks)
endif()

if(${ieompp_ENABLE_DOC})
    add_subdirectory(doc)
endif()

git_submodule(external/quicli)
add_subdirectory(src)

file(GLOB python_scripts "${CMAKE_SOURCE_DIR}/scripts/*.py")
install(PROGRAMS ${python_scripts} DESTINATION bin)
