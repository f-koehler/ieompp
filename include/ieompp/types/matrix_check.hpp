#ifndef IEOMPP_TYPES_MATRIX_CHECK_HPP_
#define IEOMPP_TYPES_MATRIX_CHECK_HPP_

#include <type_traits>

#include "ieompp/types/matrix.hpp"

namespace ieompp
{
    namespace types
    {
        template <typename Matrix>
        bool is_quadratic(const Matrix& m)
        {
            return MatrixDimensionInfo<Matrix>::rows(m) == MatrixDimensionInfo<Matrix>::columns(m);
        }

        template <typename Matrix>
        bool is_symmetric(const Matrix& m)
        {
            using Index = typename IndexType<Matrix>::Index;

            if(!is_quadratic(m)) {
                return false;
            }

            const Index N = MatrixDimensionInfo<Matrix>::rows(m);
            for(Index i = 0; i < N; ++i) {
                for(Index j = 0; j < i; ++j) {
                    if(!IsEqual(m(i, j), m(j, i))) {
                        return false;
                    }
                }
            }

            return true;
        }

        template <typename Matrix>
        bool is_hermitian(const Matrix& m)
        {
            using Index = typename IndexType<Matrix>::Index;

            if(!is_quadratic(m)) {
                return false;
            }

            const Index N = MatrixDimensionInfo<Matrix>::rows(m);
            for(Index i = 0; i < N; ++i) {
                for(Index j = 0; j <= i; ++j) {
                    if(!IsEqual(m(i, j), conjugate(m(i, j)))) {
                        return false;
                    }
                }
            }

            return true;
        }

        template <typename Matrix>
        typename std::enable_if<IsMatrix<Matrix>::value, bool>::type IsZero(const Matrix& m)
        {
            using Index     = typename IndexType<Matrix>::Index;
            const auto rows = MatrixDimensionInfo<Matrix>::rows(m);
            const auto cols = MatrixDimensionInfo<Matrix>::columns(m);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    if(!IsZero(t(i, j))) {
                        return false;
                    }
                }
            }
            return true;
        }
    } // namespace types
} // namespace ieompp

#endif
