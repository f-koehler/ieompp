#ifndef IEOMPP_TYPES_EIGEN_CHECK_HPP_
#define IEOMPP_TYPES_EIGEN_CHECK_HPP_

#include <type_traits>

#include <ieompp/types/description.hpp>
#include <ieompp/types/eigen/traits.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Matrix>
        bool is_quadratic(const Matrix& m)
        {
            return m.rows() == m.cols();
        }

        template <typename Matrix>
        typename std::enable_if<is_eigen_matrix<Matrix>::value, bool>::type
        is_symmetric(const Matrix& m)
        {
            using Index = typename index_type<Matrix>::Index;

            if(!is_quadratic(m)) return false;

            const Index N = m.rows();
            for(Index i = 0; i < N; ++i) {
                for(Index j = 0; j < i; ++j) {
                    if(!is_equal(m(i, j), m(j, i))) return false;
                }
            }

            return true;
        }

        template <typename Matrix>
        typename std::enable_if<is_eigen_matrix<Matrix>::value, bool>::type
        is_hermitian(const Matrix& m)
        {
            using Index = typename index_type<Matrix>::Index;

            if(!is_quadratic(m)) return false;

            const Index N = m.rows();
            for(Index i = 0; i < N; ++i) {
                for(Index j = 0; j <= i; ++j) {
                    if(!is_equal(m(i, j), conjugate(m(i, j)))) return false;
                }
            }

            return true;
        }

        template <typename T>
        typename std::enable_if<is_eigen_dense_matrix<T>::value, bool>::type is_zero(const T& t)
        {
            using Index = typename index_type<T>::Index;
            for(Index i = 0; i < t.rows(); ++i) {
                for(Index j = 0; j < t.cols(); ++j) {
                    if(!is_zero(t(i, j))) return false;
                }
            }
            return true;
        }
    }
}

#endif
