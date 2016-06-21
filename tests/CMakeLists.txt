include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/external/catch/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

function(add_ieompp_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} "${TEST_SOURCE}")
    add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

add_subdirectory(algebra)
