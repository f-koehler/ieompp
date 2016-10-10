#ifndef IEOMPP_TYPES_EIGEN_TRAITS_HPP_
#define IEOMPP_TYPES_EIGEN_TRAITS_HPP_

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <ieompp/types/matrix.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Cols, int Options, int MaxCols>
        struct is_matrix<Eigen::Matrix<Scalar, 1, Cols, Options, 1, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Options, int MaxRows>
        struct is_matrix<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_matrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Cols, int Options, int MaxCols>
        struct is_vector<Eigen::Matrix<Scalar, 1, Cols, Options, 1, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Options, int MaxRows>
        struct is_vector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_vector<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_row_vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Cols, int Options, int MaxCols>
        struct is_row_vector<Eigen::Matrix<Scalar, 1, Cols, Options, 1, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_row_vector<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_column_vector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Options, int MaxRows>
        struct is_column_vector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_column_vector<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct scalar_type<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            using type = Scalar;
        };

        template <typename Scalar, int Options, typename Index>
        struct scalar_type<Eigen::SparseMatrix<Scalar, Options, Index>> {
            using type = Scalar;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct index_type<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            using type = int;
        };

        template <typename Scalar, int Options, typename Index>
        struct index_type<Eigen::SparseMatrix<Scalar, Options, Index>> {
            using type = Index;
        };


        template <typename T>
        struct is_eigen_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_eigen_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_eigen_matrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = false;
        };


        template <typename T>
        struct is_eigen_dense_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_eigen_dense_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_eigen_dense_matrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = false;
        };


        template <typename T>
        struct is_eigen_sparse_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_eigen_sparse_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_eigen_sparse_matrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };
    }
}

#endif
