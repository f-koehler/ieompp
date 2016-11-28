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

add_executable(
    lattices.linear
    lattices/test_linear.cpp
)

add_executable(
    models.hubbard_common.dispersion
    models/hubbard_common/test_dispersion.cpp
)

add_executable(
    models.hubbard_momentum_space.excited_fermi_sea
    models/hubbard_momentum_space/test_excited_fermi_sea.cpp
)

add_ieompp_test(algebra.monomial)
add_ieompp_test(algebra.operator)
add_ieompp_test(lattices.linear)
add_ieompp_test(models.hubbard_common.dispersion)
add_ieompp_test(models.hubbard_momentum_space.excited_fermi_sea)
