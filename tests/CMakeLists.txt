include_directories(${CMAKE_SOURCE_DIR}/include)

function(add_ieompp_test test_name)
    target_link_libraries(${test_name} ${Boost_LIBRARIES})
    add_test(${test_name} ${test_name})
endfunction()

add_executable(
    algebra.monomial
    algebra/test_monomial.cpp
    algebra/monomial/comparison.cpp
    algebra/monomial/conjugate.cpp
    algebra/monomial/multiply.cpp
)
add_ieompp_test(algebra.monomial)

add_executable(
    algebra.operator
    algebra/test_operator.cpp
    algebra/operator/comparison.cpp
    algebra/operator/conjugate.cpp
    algebra/operator/get_index.cpp
    algebra/operator/index_tuple.cpp
    algebra/operator/make.cpp
    algebra/operator/same_indices.cpp
)
add_ieompp_test(algebra.operator)
