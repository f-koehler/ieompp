#ifndef IEOMPP_TYPES_MATRIX_CHECK_HPP_
#define IEOMPP_TYPES_MATRIX_CHECK_HPP_

#include <type_traits>

#include <ieompp/types/matrix.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Matrix>
        bool is_quadratic(const Matrix& m)
        {
            return matrix_dimension_info<Matrix>::rows(m) == matrix_dimension_info<Matrix>::columns(m);
        }

        template <typename Matrix>
        bool is_symmetric(const Matrix& m)
        {
            using Index = typename index_type<Matrix>::Index;

            if(!is_quadratic(m)) return false;

            const Index N = matrix_dimension_info<Matrix>::rows(m);
            for(Index i = 0; i < N; ++i) {
                for(Index j = 0; j < i; ++j) {
                    if(!is_equal(m(i, j), m(j, i))) return false;
                }
            }

            return true;
        }

        template <typename Matrix>
        bool is_hermitian(const Matrix& m)
        {
            using Index = typename index_type<Matrix>::Index;

            if(!is_quadratic(m)) return false;

            const Index N = matrix_dimension_info<Matrix>::rows(m);
            for(Index i = 0; i < N; ++i) {
                for(Index j = 0; j <= i; ++j) {
                    if(!is_equal(m(i, j), conjugate(m(i, j)))) return false;
                }
            }

            return true;
        }

        template <typename Matrix>
        typename std::enable_if<is_matrix<Matrix>::value, bool>::type is_zero(const Matrix& m)
        {
            using Index = typename index_type<Matrix>::Index;
            const auto rows = matrix_dimension_info<Matrix>::rows(m);
            const auto cols = matrix_dimension_info<Matrix>::columns(m);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    if(!is_zero(t(i, j))) return false;
                }
            }
            return true;
        }
    }
}

#endif
