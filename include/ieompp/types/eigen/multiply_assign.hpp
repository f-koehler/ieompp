#ifndef IEOMPP_TYPES_EIGEN_MULTIPLY_ASSIGN_HPP_
#define IEOMPP_TYPES_EIGEN_MULTIPLY_ASSIGN_HPP_

#include <Eigen/Dense>

#include <ieompp/types/function_matrix.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Scalar, typename Index, int Options, int MaxRows, int MaxCols>
        void
        multiply_assign(const FunctionMatrix<Scalar, Index>& matrix,
                        Eigen::Matrix<Scalar, Eigen::Dynamic, 1, Options, MaxRows, MaxCols>& vector)
        {
            assert(is_quadratic(matrix));
            assert(matrix.cols() == vector.rows());

            const Index rows = matrix.rows();
            const Index cols = matrix.cols();

#ifdef _OPENMP
#pragma omp parallel for
#endif
            for(Index row = 0; row < rows; ++row) {
                vector(row) = 0;
                for(Index col = 0; col < cols; ++col) {
                    vector(row) += matrix(row, col) * vector(col);
                }
            }
        }
    }
}

#endif
