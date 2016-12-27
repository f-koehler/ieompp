add_executable(
    models.hubbard_real_space.basis3
    test_basis3.cpp
    basis3/init.cpp
    basis3/get_3op_index.cpp
)
add_executable(
    models.hubbard_real_space.expectation_value
    test_expectation_value.cpp
    expectation_value/1d_half_filled.cpp
    expectation_value/2d_half_filled.cpp
)
add_executable(models.hubbard_real_space.fermi_jump test_fermi_jump.cpp)
add_executable(models.hubbard_real_space.site_occupation test_site_occupation.cpp)

add_ieompp_test(models.hubbard_real_space.basis3)
add_ieompp_test(models.hubbard_real_space.expectation_value)
add_ieompp_test(models.hubbard_real_space.fermi_jump)
add_ieompp_test(models.hubbard_real_space.site_occupation)
