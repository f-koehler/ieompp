add_executable(
    models.hubbard_momentum_space.excited_fermi_sea
    test_excited_fermi_sea.cpp
    excited_fermi_sea/annihilate_particle_1d.cpp
    excited_fermi_sea/create_particle_1d.cpp
    excited_fermi_sea/is_initially_occupied_1d.cpp
    excited_fermi_sea/particle_number_1d.cpp
)

add_executable(
    models.hubbard_momentum_space.particle_number
    test_particle_number.cpp
)

add_ieompp_test(models.hubbard_momentum_space.excited_fermi_sea)
add_ieompp_test(models.hubbard_momentum_space.particle_number)
