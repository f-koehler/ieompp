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
        typename std::enable_if<is_sparse_eigen_matrix<Matrix>::value, void>::type init(
            Matrix& m, std::size_t rows, std::size_t cols,
            std::function<typename eigen_matrix_traits<Matrix>::Scalar(std::size_t, std::size_t)> f)
        {
            m = Matrix(rows, cols);
            for(std::size_t i = 0; i < rows; ++i) {
                for(std::size_t j = 0; j < cols; ++j) {
                    const auto val = f(i, j);
                    if(!is_zero(val)) m.insert(i, j) = val;
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_sparse_eigen_matrix<Matrix>::value, void>::type init_symmetric(
            Matrix& m, std::size_t rows,
            std::function<typename eigen_matrix_traits<Matrix>::Scalar(std::size_t, std::size_t)> f)
        {
            m = Matrix(rows, rows);
            for(std::size_t i = 0; i < rows; ++i) {
                for(std::size_t j = 0; j < i; ++j) {
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
        typename std::enable_if<!is_sparse_eigen_matrix<Matrix>::value, void>::type init(
            Matrix& m, std::size_t rows, std::size_t cols,
            std::function<typename eigen_matrix_traits<Matrix>::Scalar(std::size_t, std::size_t)> f)
        {
            m = Matrix::Zero(rows, cols);
            for(std::size_t i = 0; i < rows; ++i) {
                for(std::size_t j = 0; j < cols; ++j) {
                    m(i, j) = f(i, j);
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<!is_sparse_eigen_matrix<Matrix>::value, void>::type init_symmetric(
            Matrix& m, std::size_t rows,
            std::function<typename eigen_matrix_traits<Matrix>::Scalar(std::size_t, std::size_t)> f)
        {
            m = Matrix::Zero(rows, rows);
            for(std::size_t i = 0; i < rows; ++i) {
                for(std::size_t j = 0; j < i; ++j) {
                    const auto val = f(i, j);
                    m(i, j) = val;
                    m(j, i) = val;
                }
                m(i, i) = f(i, i);
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_sparse_eigen_matrix<Matrix>::value, void>::type init_parallel(
            Matrix& m, std::size_t rows, std::size_t cols,
            std::function<typename eigen_matrix_traits<Matrix>::Scalar(std::size_t, std::size_t)> f)
        {
#if defined(_OPENMP)
            m = Matrix(rows, cols);

            omp_lock_t write_lock;
            omp_init_lock(&write_lock);

#pragma omp parallel for
            for(std::size_t i = 0; i < rows; ++i) {
                std::vector<typename eigen_matrix_traits<Matrix>::Triplet> triplets;
                triplets.reserve(1);
                auto write_back = [&m, &triplets, &write_lock]() {
                    if(triplets.empty()) return;
                    omp_set_lock(&write_lock);
                    m.setFromTriplets(triplets.begin(), triplets.end());
                    omp_unset_lock(&write_lock);
                    triplets.clear();
                };
                for(std::size_t j = 0; j < cols; ++j) {
                    triplets.emplace_back(i, j, f(i, j));
                    if(triplets.size() == triplets.capacity()) write_back();
                }
                write_back();
            }

            omp_destroy_lock(&write_lock);
#else
            init<Matrix>(rows, cols, f);
#endif
        }
    }
}

#endif
