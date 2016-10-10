#ifndef IEOMPP_TYPES_BLAZE_MULTIPLY_ASSIGN_HPP_
#define IEOMPP_TYPES_BLAZE_MULTIPLY_ASSIGN_HPP_

#include <cassert>

#include <blaze/math/DynamicVector.h>

#include <ieompp/types/function_matrix.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Scalar, typename Index>
        void multiply_assign(const FunctionMatrix<Scalar, Index>& matrix,
                             blaze::DynamicVector<Scalar, blaze::columnVector>& vector)
        {
            assert(is_quadratic(matrix));
            assert(matrix.rows() == vector.size());

            const Index rows = matrix.rows();

            static blaze::DynamicVector<Scalar, blaze::columnVector> temp;
            temp.resize(rows, false);
            temp.reset();

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
