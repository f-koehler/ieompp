include_directories(${CMAKE_SOURCE_DIR}/external/catch/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

function(add_hubbard_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} "${TEST_SOURCE}")
    if(${hubbard_ENABLE_COVERAGE})
        target_link_libraries(${TEST_NAME} gcov)
        set_source_files_properties(${TEST_SOURCE} PROPERTIES COMPILE_FLAGS "-fno-inline -fno-inline-small-functions -fno-default-inline --coverage")
    else()
        set_source_files_properties(${TEST_SOURCE} PROPERTIES COMPILE_FLAGS "-fno-inline -fno-inline-small-functions -fno-default-inline")
    endif()
    add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

add_hubbard_test("types" test_types.cpp)

add_subdirectory(abstract)
add_subdirectory(algebra)
