cmake_minimum_required(VERSION 2.8)
project(ieompp)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# enable C++14 standard
include(cmake/cpp14.cmake)

# enable warnings
include(cmake/warnings.cmake)

# produce native binaries
if(NOT ${ieompp_DISABLE_NATIVE})
    include(cmake/native.cmake)
endif()

# optimization of debug builds
if(CMAKE_BUILD_TYPE MATCHES Debug)
    include(cmake/debug.cmake)
endif()

# disable RTTI
include(cmake/disable-rtti.cmake)

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
if(${ieompp_OVERRIDE_BOOST})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_program_options -lboost_unit_test_framework")
else()
    find_package(Boost REQUIRED COMPONENTS program_options unit_test_framework)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
endif()

# enable OpenMP if possible
option(ieompp_ENABLE_OPENMP "Enable OpenMP" ON)
if(${ieompp_ENABLE_OPENMP})
    option(ieompp_USE_INTEL_OPENMP "Use Intel OpenMP implementation" OFF)
    if(${ieompp_USE_INTEL_OPENMP})
        include(cmake/intel-openmp.cmake)
    else()
        find_package(OpenMP REQUIRED)
        if(${OPENMP_FOUND})
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        endif()
    endif()
endif()

# enable Intel MKL
option(ieompp_ENABLE_MKL "Enable MKL" OFF)
if(${ieompp_ENABLE_MKL})
    set(MKLROOT "/opt/intel/mkl/")
    if(NOT ${ieompp_ENABLE_OPENMP})
        include(cmake/intel-mkl-serial.cmake)
    else()
        if(${ieompp_USE_INTEL_OPENMP})
            include(cmake/intel-mkl-iomp.cmake)
        else()
            include(cmake/intel-mkl-gomp.cmake)
        endif()
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
