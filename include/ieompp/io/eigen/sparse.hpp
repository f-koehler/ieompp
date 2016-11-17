#ifndef IEOMPP_IO_EIGEN_SPARSE_HPP_
#define IEOMPP_IO_EIGEN_SPARSE_HPP_

#include "ieompp/io/exception.hpp"
#include "ieompp/io/line.hpp"
#include "ieompp/string.hpp"
#include "ieompp/types/eigen.hpp"

#include <cstdlib>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace ieompp
{
    namespace io
    {
        template <typename Matrix>
        typename std::enable_if<types::IsEigenSparseMatrix<Matrix>::value, void>::type
        write_matrix(std::ostream& strm, const Matrix& m)
        {
            using Index = typename types::index_type<Matrix>::type;

            for(Index i = 0; i < m.outerSize(); ++i) {
                for(typename Matrix::InnerIterator it(m, i); it; ++it) {
                    strm << it.row() << '\t' << it.col() << '\t' << it.value() << '\n';
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<types::IsEigenSparseMatrix<Matrix>::value, void>::type
        read_matrix(std::istream& strm, Matrix& m, typename Matrix::Index nnz_per_inner_vec)
        {
            static const std::regex dimension_reg("^\\s*(\\d+)\\s*x\\s*(\\d+)\\s*$");

            auto line = next_line(strm);
            std::smatch match;
            if(!std::regex_match(line, match, dimension_reg)) {
                THROW(IOException,
                      "Expected a line of the type \"123x456\" specifying the matrix dimensions");
            }

            auto rows = std::strtoul(match[1].str().c_str(), nullptr, 10);
            auto cols = std::strtoul(match[2].str().c_str(), nullptr, 10);
            m         = Matrix(rows, cols);
            m.reserve(Eigen::VectorXi::Constant(cols, nnz_per_inner_vec));

            using Index  = typename Matrix::Index;
            using Scalar = typename Matrix::Scalar;

            line = next_line(strm);

            Index num_entries = std::strtoul(line.c_str(), nullptr, 10);
            Index i, j;
            Scalar val;
            for(Index n = 0; n < num_entries; ++n) {
                std::istringstream ss(next_line(strm));
                ss >> i >> j >> val;
                m.insert(i, j) = val;
            }
        }
    }
}

#endif
