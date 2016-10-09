#ifndef IEOMPP_TYPES_EIGEN_FUNCTION_MATRIX_INTEROP_HPP_
#define IEOMPP_TYPES_EIGEN_FUNCTION_MATRIX_INTEROP_HPP_

#include <functional>

#if defined(_OPENMP)
#include <omp.h>
#endif

#include <Eigen/Dense>

#include <ieompp/types/function_matrix.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Scalar, typename Index, int Options, int MaxRows, int MaxCols>
        struct FunctionMatrixEigenProduct {
            std::reference_wrapper<const FunctionMatrix<Scalar, Index>> matrix_ref;
            std::reference_wrapper<const Eigen::Matrix<Scalar, Eigen::Dynamic, 1, Options, MaxRows,
                                                       MaxCols>>
                vector_ref;
        };

        template <typename Scalar, typename Index, int Options, int MaxRows, int MaxCols>
        FunctionMatrixEigenProduct<Scalar, Index, Options, MaxRows, MaxCols>
        operator*(const FunctionMatrix<Scalar, Index>& m,
                  const Eigen::Matrix<Scalar, Eigen::Dynamic, 1, Options, MaxRows, MaxCols>& v)
        {
            assert(v.rows() == m.cols());
            return {std::cref(m), std::cref(v)};

            /* const auto rows = m.rows(); */
            /* const auto cols = m.cols(); */

            /* assert(v.rows() == cols); */

            /* Eigen::Matrix<Scalar, Eigen::Dynamic, 1, Options, MaxRows, MaxCols> result(rows); */
            /* result.setZero(); */

            /* for(Index row = 0; row < rows; ++row) { */
            /*     for(Index col = 0; col < cols; ++col) { */
            /*         result(row) += m(row, col) * v(col); */
            /*     } */
            /* } */
            /* return result; */
        }
    }
}

#endif
