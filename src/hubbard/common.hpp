#ifndef SRC_COMMON_HPP_
#define SRC_COMMON_HPP_

#include <complex>

#include <blaze/math/serialization/MatrixSerializer.h>
#include <blaze/util/Serialization.h>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/ode/rk4.hpp>

#include "checkpoint.hpp"
#include "logging.hpp"

using Operator1d = ieompp::algebra::Operator<uint64_t, bool>;
using Operator2d = ieompp::algebra::Operator<uint64_t, uint64_t, bool>;
using Term1d     = ieompp::algebra::Term<double, Operator1d>;
using Term2d     = ieompp::algebra::Term<double, Operator2d>;


template <typename Scalar, bool StorageOrder>
void write_matrix_file(const std::string& path,
                       const blaze::CompressedMatrix<Scalar, StorageOrder>& matrix)
{
    get_loggers().io->info("Write matrix {}x{} matrix to \"{}\"", matrix.rows(), matrix.columns(),
                           path);
    std::ofstream strm(path.c_str(), std::ofstream::binary);
    blaze::Archive<std::ofstream> archive(strm);
    archive << matrix;
    strm.flush();
    strm.close();
    get_loggers().io->info("Finished writing matrix file \"{}\"", path);
}

template <typename Float>
ieompp::ode::RK4<Float> init_rk4(std::size_t basis_size, const Float& dt)
{
    get_loggers().ode->info("Init RK4 integrator");
    ieompp::ode::RK4<Float> rk4(basis_size, dt);
    get_loggers().ode->info("Finished RK4 initializing integrator");
    return rk4;
}

#endif
