#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "../include/logging.hpp"

#include <blaze/Blaze.h>

template <typename Basis>
auto init_vector(const Basis& basis)
{
    get_loggers().main->info(u8"Create {} dimensional vector with initial conditions (1, 0, 0, …)",
                             basis.size());
    blaze::DynamicVector<std::complex<double>> vec(basis.size());
    vec.reset();
    vec[0] = 1.;
    get_loggers().main->info("Finish creating vector");
    return vec;
}

#endif
