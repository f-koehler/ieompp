#ifndef IEOMPP_TYPES_EIGEN_MULTIPLY_ASSIGN_HPP_
#define IEOMPP_TYPES_EIGEN_MULTIPLY_ASSIGN_HPP_

#include "ieompp/types/function_matrix.hpp"

#include <cassert>

#include <Eigen/Dense>

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

            static Eigen::Matrix<Scalar, Eigen::Dynamic, 1, Options, MaxRows, MaxCols> temp;
            temp.resize(vector.rows());
            temp.setZero();

#ifdef _OPENMP
#pragma omp parallel for
#endif
            for(Index row = 0; row < rows; ++row) {
                auto& element = temp(row);
                for(Index col = 0; col < rows; ++col) {
                    element += matrix(row, col) * vector(col);
                }
            }

            vector.swap(temp);
        }
    }
}

#endif
