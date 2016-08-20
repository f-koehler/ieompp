include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
    algebra test_algebra.cpp
    algebra/operator_creation.cpp
    algebra/operator_index_tuple.cpp
    algebra/operator_same_indices.cpp
)
target_link_libraries(algebra ${Boost_LIBRARIES})
add_test(algebra algebra)
