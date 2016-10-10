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

# optimization of debug builds
if(CMAKE_BUILD_TYPE MATCHES Debug)
    include(cmake/debug.cmake)
endif()

# disable RTTI
include(cmake/disable-rtti.cmake)

# option to disable exceptions
# TODO: currently spdlog does not allow to compile without exceptions
# option(ieompp_ENABLE_EXCEPTIONS "Enable exceptions" ON)
# if(NOT ${ieompp_ENABLE_EXCEPTIONS})
#     include(cmake/disable-exceptions.cmake)
# endif()

# create version.hpp
include(cmake/version-script.cmake)
run_version_script(${CMAKE_SOURCE_DIR}/tools/configure.py ${CMAKE_SOURCE_DIR}/ieompp/include/version.hpp)

# include spdlog
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/spdlog/include)

# include Eigen
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/eigen)

# include blaze
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/blaze)

# use boost
find_package(Boost REQUIRED COMPONENTS program_options unit_test_framework)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# enable OpenMP if possible
option(ieompp_ENABLE_OPENMP "Enable OpenMP" ON)
if(${ieompp_ENABLE_OPENMP})
    find_package(OpenMP REQUIRED)
    if(${OPENMP_FOUND})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()

# option to build documentation using sphinx
option(ieompp_ENABLE_DOCUMENTATION "Enable html documentation built with sphinx" OFF)
if(${ieompp_ENABLE_DOCUMENTATION})
    find_program(SPHINX_EXECUTABLE sphinx-build)
    add_custom_target(doc
        COMMAND ${SPHINX_EXECUTABLE} -b html . ${CMAKE_BINARY_DIR}/doc
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
        COMMENT "Build documentation using sphinx"
    )
endif()

# option to use include-what-you-use
option(ieompp_ENABLE_IWYU "Enable include-what-you-use" OFF)
if(${ieompp_ENABLE_IWYU})
    include(cmake/iwyu.cmake)
else()
    function(enable_iwyu TARGET)
    endfunction()
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
