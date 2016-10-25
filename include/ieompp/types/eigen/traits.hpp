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
        struct IsMatrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Cols, int Options, int MaxCols>
        struct IsMatrix<Eigen::Matrix<Scalar, 1, Cols, Options, 1, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Options, int MaxRows>
        struct IsMatrix<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsMatrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IsVector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Cols, int Options, int MaxCols>
        struct IsVector<Eigen::Matrix<Scalar, 1, Cols, Options, 1, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Options, int MaxRows>
        struct IsVector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsVector<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IsRowVector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Cols, int Options, int MaxCols>
        struct IsRowVector<Eigen::Matrix<Scalar, 1, Cols, Options, 1, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsRowVector<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IsColumnVector<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Options, int MaxRows>
        struct IsColumnVector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsColumnVector<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct ScalarType<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            using type = Scalar;
        };

        template <typename Scalar, int Options, typename Index>
        struct ScalarType<Eigen::SparseMatrix<Scalar, Options, Index>> {
            using type = Scalar;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IndexType<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            using type = int;
        };

        template <typename Scalar, int Options, typename Index>
        struct IndexType<Eigen::SparseMatrix<Scalar, Options, Index>> {
            using type = Index;
        };


        template <typename T>
        struct IsEigenMatrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IsEigenMatrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsEigenMatrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = false;
        };


        template <typename T>
        struct IsEigenDenseMatrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IsEigenDenseMatrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsEigenDenseMatrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = false;
        };


        template <typename T>
        struct IsEigenSparseMatrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct IsEigenSparseMatrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Options, typename Index>
        struct IsEigenSparseMatrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };
    }
}

#endif
