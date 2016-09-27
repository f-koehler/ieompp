#ifndef IEOMPP_TYPES_EIGEN_HPP_
#define IEOMPP_TYPES_EIGEN_HPP_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#pragma GCC diagnostic pop

#include <ieompp/description.hpp>
#include <ieompp/types/description.hpp>

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
        struct is_eigen_row_vector
        {
            static constexpr bool value =
                detail::is_eigen_row_vector_helper<T, is_eigen_matrix<T>::value>::value;
        };

        template <typename T>
        struct is_eigen_column_vector
        {
            static constexpr bool value =
                detail::is_eigen_column_vector_helper<T, is_eigen_matrix<T>::value>::value;
        };

        template <typename T>
        struct is_eigen_vector {
            static constexpr bool value =
                is_eigen_row_vector<T>::value || is_eigen_column_vector<T>::value;
        };

        template <typename T>
        struct scalar_type
        {
            using type = typename detail::scalar_type_helper<T, is_eigen_matrix<T>::value>::type;
        };


        template <typename T>
        typename std::enable_if<is_dense_eigen_matrix<T>::value, bool>::type is_zero(const T& t)
        {
            using Index = typename eigen_matrix_traits<T>::Index;
            for(Index i = 0; i < t.rows(); ++i) {
                for(Index j = 0; j < t.cols(); ++j) {
                    if(!is_zero(t(i, j))) return false;
                }
            }
            return true;
        }
    }

    template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct VariableDescription<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Description
        get(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& m)
        {
            return {{"Dense matrix", ""},
                    {"  rows", std::to_string(m.rows())},
                    {"  cols", std::to_string(m.cols())},
                    {"  scalar", compose(TypeProperties<Scalar>::name, ' ', "(size ",
                                         TypeProperties<Scalar>::size, ")")}};
        }
    };

    template <typename Scalar, int Options, typename Index>
    struct VariableDescription<Eigen::SparseMatrix<Scalar, Options, Index>> {
        static Description get(const Eigen::SparseMatrix<Scalar, Options, Index>& m)
        {
            return {{"Sparse matrix", ""},
                    {"  rows", std::to_string(m.rows())},
                    {"  cols", std::to_string(m.cols())},
                    {"  scalar", compose(TypeProperties<Scalar>::name, ' ', "(size ",
                                         TypeProperties<Scalar>::size, ")")}};
        }
    };
}

#endif
