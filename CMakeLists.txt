cmake_minimum_required(VERSION 3.0)
project(hubbard)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

function(git_submodule DIR)
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${DIR}/.git" )
        message(STATUS "--------------------------------")
        message(STATUS "Submodule \"${DIR}\" not present. Try to obtain it using git ...")
        if(NOT DEFINED GIT_FOUND)
            find_package(Git)
        endif()
        if(GIT_FOUND)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${DIR} RESULT_VARIABLE RETVAL WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
            if(${RETVAL} EQUAL 0)
                message(STATUS "Obtained submodule \"${DIR}\".")
            else()
                message(FATAL_ERROR "Failed to obtain submodule \"${DIR}\"! Run \"git submodule update --init --recursive ${DIR}\" manually.")
            endif()
        else()
            message(FATAL_ERROR "Cannot find git! Run \"git submodule update --init --recursive ${DIR}\" manually.")
        endif()
        message(STATUS "--------------------------------")
    endif()
endfunction()

if(NOT DEFINED GIT_FOUND)
    find_package(Git)
endif()
if(${GIT_FOUND})
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --dirty
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE hubbard_VERSION
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    MESSAGE(STATUS "Hubbard version: ${hubbard_VERSION}")
else()
    set(hubbard_VERSION "unknown")
endif()
configure_file("${CMAKE_SOURCE_DIR}/include/hubbard/config.hpp.in" "${CMAKE_SOURCE_DIR}/include/hubbard/config.hpp")

option(hubbard_ENABLE_TESTS      "Option to enable unittests" ON)
option(hubbard_ENABLE_COVERAGE   "Option to enable code coverage measurement" OFF)
option(hubbard_ENABLE_COVERALLS  "Option to enable coverage report to coveralls.io" OFF)
option(hubbard_ENABLE_BENCHMARKS "Option to enable benchmarks" OFF)
option(hubbard_ENABLE_DOC        "Option to enable documentation" OFF)

if(${hubbard_ENABLE_TESTS})
    enable_testing()
    git_submodule(external/catch)
    add_subdirectory(tests)
endif()

if(${hubbard_ENABLE_BENCHMARKS})
    git_submodule(external/benchmark)
    add_subdirectory(benchmarks)
endif()

if(${hubbard_ENABLE_DOC})
    add_subdirectory(doc)
endif()

add_executable(main main.cpp)
add_executable(fermion_cas src/fermion_cas.cpp)
add_executable(regexes src/regexes.cpp)
add_executable(solve src/solve.cpp)

install(TARGETS fermion_cas RUNTIME DESTINATION bin)
file(GLOB python_scripts "${CMAKE_SOURCE_DIR}/scripts/*.py")
install(PROGRAMS ${python_scripts} DESTINATION bin)
