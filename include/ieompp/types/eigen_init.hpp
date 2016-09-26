#ifndef IEOMPP_TYPES_EIGEN_INIT_HPP_
#define IEOMPP_TYPES_EIGEN_INIT_HPP_

#include <functional>
#include <omp.h>

#include <ieompp/types/eigen.hpp>
#include <ieompp/types/number.hpp>

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

        template <typename Matrix>
        typename std::enable_if<is_sparse_eigen_matrix<Matrix>::value, void>::type
        init_parallel(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
                      typename eigen_matrix_traits<Matrix>::Index cols,
                      std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                          typename eigen_matrix_traits<Matrix>::Index,
                          typename eigen_matrix_traits<Matrix>::Index)>
                          f)
        {
#if defined(_OPENMP)
            using Index = typename eigen_matrix_traits<Matrix>::Index;
            m = Matrix(rows, cols);

            omp_lock_t write_lock;
            omp_init_lock(&write_lock);

#pragma omp parallel for
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < rows; ++j) {
                    const auto val = f(i, j);
                    if(!is_zero(val)) {
                        omp_set_lock(&write_lock);
                        m.insert(i, j) = val;
                        omp_unset_lock(&write_lock);
                    }
                }
            }

            /* #pragma omp parallel for */
            omp_destroy_lock(&write_lock);
#else
            init<Matrix>(rows, cols, f);
#endif
        }

        template <typename Matrix>
        typename std::enable_if<is_dense_eigen_matrix<Matrix>::value, void>::type
        init_parallel(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
                      typename eigen_matrix_traits<Matrix>::Index cols,
                      std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                          typename eigen_matrix_traits<Matrix>::Index,
                          typename eigen_matrix_traits<Matrix>::Index)>
                          f)
        {
#if defined(_OPENMP)
            using Index = typename eigen_matrix_traits<Matrix>::Index;

            m = Matrix::Zero(rows, cols);
#pragma omp parallel for
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    m(i, j) = f(i, j);
                }
            }
#else
            init_symmetric(m, rows, cols, f);
#endif
        }

        template <typename Matrix>
        typename std::enable_if<is_dense_eigen_matrix<Matrix>::value, void>::type
        init_symmetric_parallel(Matrix& m, typename eigen_matrix_traits<Matrix>::Index rows,
                                std::function<typename eigen_matrix_traits<Matrix>::Scalar(
                                    typename eigen_matrix_traits<Matrix>::Index,
                                    typename eigen_matrix_traits<Matrix>::Index)>
                                    f)
        {
#if defined(_OPENMP)
            using Index = typename eigen_matrix_traits<Matrix>::Index;

            m = Matrix::Zero(rows, rows);
#pragma omp parallel for
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < i; ++j) {
                    const auto val = f(i, j);
                    m(i, j) = val;
                    m(j, i) = val;
                }
                m(i, i) = f(i, i);
            }
#else
            init_symmetric(m, rows, f);
#endif
        }
    }
}

#endif
