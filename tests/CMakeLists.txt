include_directories(${CMAKE_SOURCE_DIR}/include)

function(add_ieompp_test test_name)
    target_link_libraries(${test_name} ${Boost_LIBRARIES})
    add_test(${test_name} ${test_name})
endfunction()

add_executable(
    algebra test_algebra.cpp
    algebra/operator.cpp
    algebra/operator_comparison.cpp
    algebra/operator_creation.cpp
    algebra/operator_get_index.cpp
    algebra/operator_index_tuple.cpp
    algebra/operator_same_indices.cpp
)
add_ieompp_test(algebra)
