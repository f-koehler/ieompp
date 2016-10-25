#ifndef IEOMPP_IO_BLAZE_SPARSE_HPP_
#define IEOMPP_IO_BLAZE_SPARSE_HPP_

#include <cstdlib>
#include <regex>
#include <string>

#include "ieompp/io/exception.hpp"
#include "ieompp/types/blaze.hpp"

#include <blaze/math/serialization/MatrixSerializer.h>
#include <blaze/util/Serialization.h>

namespace ieompp
{
    namespace io
    {
        template <typename Matrix>
        typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
        write_matrix(const std::string& path, const Matrix& matrix)
        {
            blaze::Archive<std::ofstream> archive(path);
            archive << matrix;
        }

        template <typename Matrix>
        typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
        read_matrix(const std::string& path, Matrix& matrix)
        {
            blaze::Archive<std::ifstream> archive(path);
            archive >> matrix;
        }
    } // namespace io
} // namespace ieompp

#endif
