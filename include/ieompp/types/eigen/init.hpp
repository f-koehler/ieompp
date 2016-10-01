#ifndef IEOMPP_TYPES_EIGEN_INIT_HPP_
#define IEOMPP_TYPES_EIGEN_INIT_HPP_

#include <ieompp/types/eigen/traits.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Matrix>
        typename std::enable_if<is_sparse_eigen_matrix<Matrix>::value, void>::type
        init(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
             typename eigen_matrix_traits<Matrix>::Index cols,
             std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                 typename eigen_matrix_traits<Matrix>::Index,
                 typename eigen_matrix_traits<Matrix>::Index)>
                 f)
        {
            using Index = typename eigen_matrix_traits<Matrix>::Index;

            m = Matrix(rows, cols);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    const auto val = f(i, j);
                    if(!is_zero(val)) m.insert(i, j) = val;
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_sparse_eigen_matrix<Matrix>::value, void>::type
        init_symmetric(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
                       std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                           typename eigen_matrix_traits<Matrix>::Index,
                           typename eigen_matrix_traits<Matrix>::Index)>
                           f)
        {
            using Index = typename eigen_matrix_traits<Matrix>::Index;

            m = Matrix(rows, rows);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < i; ++j) {
                    const auto val = f(i, j);
                    if(!is_zero(val)) {
                        m.insert(i, j) = val;
                        m.insert(j, i) = val;
                    }
                }
                const auto val = f(i, i);
                if(!is_zero(val)) m.insert(i, i) = val;
            }
        }

        template <typename Container>
        typename std::enable_if<is_eigen_triplet<typename Container::value_type>::value, void>::type
        init(Container& c, typename is_eigen_triplet<typename Container::value_type>::Index rows,
             typename is_eigen_triplet<typename Container::value_type>::Index cols,
             std::function<typename is_eigen_triplet<typename Container::value_type>::Scalar(
                 typename is_eigen_triplet<typename Container::value_type>::Scalar,
                 typename is_eigen_triplet<typename Container::value_type>::Scalar)>
                 f)
        {
            using Index = typename is_eigen_triplet<typename Container::value_type>::Index;

            c.clear();

            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    const auto val = f(i, j);
                    if(!is_zero(val)) {
                        c.emplace_back(i, j, val);
                    }
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_dense_eigen_matrix<Matrix>::value, void>::type
        init(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
             typename eigen_matrix_traits<Matrix>::Index cols,
             std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                 typename eigen_matrix_traits<Matrix>::Index,
                 typename eigen_matrix_traits<Matrix>::Index)>
                 f)
        {
            using Index = typename eigen_matrix_traits<Matrix>::Index;

            m = Matrix::Zero(rows, cols);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    m(i, j) = f(i, j);
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_dense_eigen_matrix<Matrix>::value, void>::type
        init_symmetric(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
                       std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                           typename eigen_matrix_traits<Matrix>::Index,
                           typename eigen_matrix_traits<Matrix>::Index)>
                           f)
        {
            using Index = typename eigen_matrix_traits<Matrix>::Index;

            m = Matrix::Zero(rows, rows);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < i; ++j) {
                    const auto val = f(i, j);
                    m(i, j) = val;
                    m(j, i) = val;
                }
                m(i, i) = f(i, i);
            }
        }
    }
}

#endif
