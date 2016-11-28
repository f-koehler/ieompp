add_executable(
    algebra.monomial
    test_monomial.cpp
    monomial/comparison.cpp
    monomial/conjugate.cpp
    monomial/multiply.cpp
)

add_executable(
    algebra.operator
    test_operator.cpp
    operator/comparison.cpp
    operator/conjugate.cpp
    operator/get_index.cpp
    operator/index_tuple.cpp
    operator/make.cpp
    operator/same_indices.cpp
)

add_ieompp_test(algebra.monomial)
add_ieompp_test(algebra.operator)
