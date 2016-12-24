add_executable(
    models.hubbard.dispersion
    test_dispersion.cpp
    dispersion/1d.cpp
    dispersion/2d.cpp
)

add_ieompp_test(models.hubbard.dispersion)
