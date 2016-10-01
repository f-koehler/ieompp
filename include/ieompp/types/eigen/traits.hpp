#ifndef IEOMPP_TYPES_EIGEN_TRAITS_HPP_
#define IEOMPP_TYPES_EIGEN_TRAITS_HPP_

#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace ieompp
{
    namespace types
    {
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
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_dense_eigen_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_dense_eigen_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_dense_eigen_matrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_sparse_eigen_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_sparse_eigen_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows,
                                                    MaxCols>> {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Options, typename Index>
        struct is_sparse_eigen_matrix<Eigen::SparseMatrix<Scalar, Options, Index>> {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_eigen_triplet {
            static constexpr bool value = false;
        };

        template <typename ScalarT, typename IndexT>
        struct is_eigen_triplet<Eigen::Triplet<ScalarT, IndexT>> {
            using Scalar = ScalarT;
            using Index  = IndexT;

            static constexpr bool value = true;
        };

        template <typename T>
        struct eigen_matrix_traits {
        };

        template <typename ScalarT, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct eigen_matrix_traits<Eigen::Matrix<ScalarT, Rows, Cols, Options, MaxRows, MaxCols>> {
            using Scalar = ScalarT;
            using Index =
                typename Eigen::Matrix<ScalarT, Rows, Cols, Options, MaxRows, MaxCols>::Index;

            static constexpr auto rows     = Rows;
            static constexpr auto columns  = Cols;
            static constexpr auto options  = Options;
            static constexpr auto max_rows = MaxRows;
            static constexpr auto max_cols = MaxCols;
        };

        template <typename ScalarT, int Options, typename IndexT>
        struct eigen_matrix_traits<Eigen::SparseMatrix<ScalarT, Options, IndexT>> {
            using Scalar  = ScalarT;
            using Index   = IndexT;
            using Triplet = Eigen::Triplet<Scalar, Index>;

            static constexpr auto options = Options;
        };

        namespace detail
        {
            template <typename T, bool is_matrix>
            struct is_eigen_row_vector_helper;

            template <typename T>
            struct is_eigen_row_vector_helper<T, false> {
                static constexpr bool value = false;
            };

            template <typename T>
            struct is_eigen_row_vector_helper<T, true> {
                static constexpr bool value = (eigen_matrix_traits<T>::rows == 1);
            };

            template <typename T, bool is_matrix>
            struct is_eigen_column_vector_helper;

            template <typename T>
            struct is_eigen_column_vector_helper<T, false> {
                static constexpr bool value = false;
            };

            template <typename T>
            struct is_eigen_column_vector_helper<T, true> {
                static constexpr bool value = (eigen_matrix_traits<T>::columns == 1);
            };

            template <typename T, bool is_matrix>
            struct scalar_type_helper;

            template <typename T>
            struct scalar_type_helper<T, false> {
                using type = T;
            };

            template <typename T>
            struct scalar_type_helper<T, true> {
                using type = typename eigen_matrix_traits<T>::Scalar;
            };
        }

        template <typename T>
        struct is_eigen_row_vector {
            static constexpr bool value =
                detail::is_eigen_row_vector_helper<T, is_eigen_matrix<T>::value>::value;
        };

        template <typename T>
        struct is_eigen_column_vector {
            static constexpr bool value =
                detail::is_eigen_column_vector_helper<T, is_eigen_matrix<T>::value>::value;
        };

        template <typename T>
        struct is_eigen_vector {
            static constexpr bool value =
                is_eigen_row_vector<T>::value || is_eigen_column_vector<T>::value;
        };

        template <typename T>
        struct scalar_type {
            using type = typename detail::scalar_type_helper<T, is_eigen_matrix<T>::value>::type;
        };
    }
}

#endif
