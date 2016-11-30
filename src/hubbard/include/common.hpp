#ifndef SRC_COMMON_HPP_
#define SRC_COMMON_HPP_

#include <complex>
#include <regex>
#include <string>

#include <blaze/math/serialization/MatrixSerializer.h>
#include <blaze/util/Serialization.h>

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/ode/rk4.hpp>

#include "application.hpp"
#include "checkpoint.hpp"
#include "logging.hpp"

using Operator = ieompp::algebra::Operator<uint64_t, bool>;
using Monomial = ieompp::algebra::Monomial<Operator>;


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

blaze::DynamicVector<std::complex<double>> read_vector_from_checkpoint(Application& app)
{
    get_loggers().main->info("Read initial vector from checkpoint file");

    blaze::DynamicVector<std::complex<double>> vec;
    read_checkpoint_file(app.variables["checkpoint"].as<std::string>(), vec);

    const std::regex re_checkpoint_file("^.*" + app.checkpoint_prefix + R"((\d+)\.blaze$)");
    std::smatch m;
    get_loggers().main->info(
        "Clean up output file to be in the state according to checkpoint for step TODO");

    // TODO
    clean_output_file(app.output_path, 42);
    get_loggers().main->info("Finished reading vector from checkpoint file");

    return vec;
}


template <typename Float>
bool has_time_interval_passed(const Float& t, const Float& last, const Float& dt, uint64_t interval)
{
    if(last >= t) return false;
    return static_cast<uint64_t>(std::round((t - last) / dt)) >= interval;
}

#endif
