#ifndef IEOMPP_TYPES_EIGEN_HPP_
#define IEOMPP_TYPES_EIGEN_HPP_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Dense>
#pragma GCC diagnostic pop

namespace ieompp
{
    namespace types
    {
        template <typename T>
        struct is_eigen_matrix
        {
            static constexpr bool value = false;
        };

        template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_eigen_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
            static constexpr bool value = true;
        };

        template <typename T>
        struct eigen_matrix_traits {
        };

        template <typename ScalarT, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct eigen_matrix_traits<Eigen::Matrix<ScalarT, Rows, Cols, Options, MaxRows, MaxCols>> {
            using Scalar = ScalarT;

            static constexpr auto rows     = Rows;
            static constexpr auto columns  = Cols;
            static constexpr auto options  = Options;
            static constexpr auto max_rows = MaxRows;
            static constexpr auto max_cols = MaxCols;
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
                static constexpr bool value = (eigen_matrix_traits<T>::cols == 1);
            };

            template <typename T, bool is_matrix>
            struct scalar_type_helper;

            template <typename T>
            struct scalar_type_helper<T, false> {
                using type = T;
            };

            template <typename T>
            struct scalar_type_helper<T, true> {
                using type = typename eigen_matrix_traits<T>::scalar;
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
    }
}

#endif
