#ifndef IEOMPP_IO_EIGEN_DENSE_HPP_
#define IEOMPP_IO_EIGEN_DENSE_HPP_

#include <cstdlib>
#include <ostream>
#include <string>

#include <ieompp/io/exception.hpp>
#include <ieompp/io/line.hpp>
#include <ieompp/string.hpp>
#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    namespace io
    {
        template <typename Matrix>
        typename std::enable_if<types::is_sparse_eigen_matrix<Matrix>::value, void>::type
        write_matrix(std::ostream& strm, const Matrix& m, bool binary = false)
        {
            using Index = typename types::eigen_matrix_traits<Matrix>::Index;

            if(!binary) {
                for(Index i = 0; i < m.outerSize(); ++i) {
                    for(typename Matrix::InnerIterator it(m, i); it; ++it) {
                        strm << it.row() << '\t' << it.col() << '\t' << it.value() << '\n';
                    }
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<types::is_sparse_eigen_matrix<Matrix>::value, void>::type
        read_matrix(std::istream& strm, Matrix& m, typename Matrix::Index nnz_per_inner_vec, bool binary = false)
        {
            static const std::regex dimension_reg("^\\s*(\\d+)\\s*x\\s*(\\d+)\\s*$");

            if(!binary) {
                std::string buf;
                do {
                    std::getline(strm, buf);
                    if(strm.fail())
                        THROW(IOException,
                              "Failed to read through skippable lines at the beginning of stream");
                    if(!is_skippable_line(buf)) break;
                } while(!strm.eof());

                std::smatch match;
                if(!std::regex_match(buf, match, dimension_reg))
                    THROW(
                        IOException,
                        "Expected a line of the type \"123x456\" specifying the matrix dimensions");


                m = Matrix(std::strtoul(match[1].str().c_str(), nullptr, 10),
                           std::strtoul(match[1].str().c_str(), nullptr, 10));
                m.reserve(Eigen::VectorXi::Constant(m.cols(), nnz_per_inner_vec));

                using Index  = typename Matrix::Index;
                using Scalar = typename Matrix::Scalar;

                Index nnz;
                strm >> nnz;
                if(strm.fail()) THROW(IOException, "Failed to determine number of entries");
                m.reserve(nnz);

                Index i, j;
                Scalar val;
                for(Index n = 1; n < nnz; ++n) {
                    strm >> i >> j >> val;
                    if(strm.fail()) THROW(IOException, compose("Failed to read element ", n));
                    m.insert(i, j) = val;
                }

                m.insert(i, j) = val;
                return;
            }
        }
    }
}

#endif
