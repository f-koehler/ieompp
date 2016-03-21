#ifndef IEOMPP_IO_EIGEN_HPP_
#define IEOMPP_IO_EIGEN_HPP_

#include "Eigen/Dense"
#include "ieompp/io/fundamental.hpp"

namespace ieompp
{
    namespace io
    {
        template <typename OStream, typename Scalar, int Rows, int Cols, int Options, int MaxRows,
                  int MaxCols>
        void write_binary(OStream& strm,
                          const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& mat)
        {
            const auto rows = mat.rows();
            const auto cols = mat.cols();
            write_binary(strm, rows);
            write_binary(strm, cols);
            for(int row = 0; row < rows; ++row) {
                for(int col = 0; col < cols; ++col) write_binary(strm, mat(row, col));
            }
        }

        template <typename IStream, typename Scalar, int Rows, int Cols, int Options, int MaxRows,
                  int MaxCols>
        void read_binary(IStream& strm,
                          const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& mat)
        {
            const auto rows = mat.rows();
            const auto cols = mat.cols();
            read_binary(strm, rows);
            read_binary(strm, cols);
            /* mat = Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::Zero(rows, cols); */
            /* for(int row = 0; row < rows; ++row) { */
            /*     for(int col = 0; col < cols; ++col) {read_binary(strm, mat(row, col)); */
            /* } */
        }
    }
}

#endif
