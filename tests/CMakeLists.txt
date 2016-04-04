include_directories(${CMAKE_SOURCE_DIR}/external/catch/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # TODO
    # the -Wno-parentheses is only a temporary workaround for Catch
    # see issue https://github.com/philsquared/Catch/issues/593
    set(TEST_EXTRA_FLAGS "-fno-inline -fno-inline-small-functions -fno-default-inline -Wno-parentheses")
endif()

function(add_ieompp_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} "${TEST_SOURCE}")
    if(${ieompp_ENABLE_COVERAGE})
        target_link_libraries(${TEST_NAME} gcov)
        set_source_files_properties(${TEST_SOURCE} PROPERTIES COMPILE_FLAGS "${TEST_CURRENT_FLAGS} ${TEST_EXTRA_FLAGS} --coverage")
    else()
        set_source_files_properties(${TEST_SOURCE} PROPERTIES COMPILE_FLAGS "${TEST_CURRENT_FLAGS} ${TEST_EXTRA_FLAGS}")
    endif()
    add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

add_subdirectory(abstract)
add_subdirectory(algebra)
add_subdirectory(discretization)
add_subdirectory(io)
