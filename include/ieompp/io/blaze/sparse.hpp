#ifndef IEOMPP_IO_BLAZE_SPARSE_HPP_
#define IEOMPP_IO_BLAZE_SPARSE_HPP_

#include <cstdlib>
#include <regex>
#include <string>

#include <ieompp/io/exception.hpp>
#include <ieompp/types/blaze/traits.hpp>

namespace ieompp
{
    namespace io
    {
        template <typename Matrix>
        typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
        write_matrix(std::ostream& strm, const Matrix& matrix)
        {
        }

        template <typename Matrix>
        typename std::enable_if<types::is_blaze_sparse_matrix<Matrix>::value, void>::type
        read_matrix(std::ostream& strm, Matrix& matrix)
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
            m.resize(rows, cols, false);

            using Index  = typename index_type<Matrix>::type;
            using Scalar = typename scalar_type<Matrix>::type;

            line = next_line(strm);

            Index num_entries = std::strtoul(line.c_str(), nullptr, 10);
            m.reserve(num_entries);

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
