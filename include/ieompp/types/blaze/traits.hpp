#ifndef IEOMPP_TYPES_BLAZE_TRAITS_HPP_
#define IEOMPP_TYPES_BLAZE_TRAITS_HPP_

#include <blaze/math/CompressedMatrix.h>
#include <blaze/math/CompressedVector.h>
#include <blaze/math/DynamicMatrix.h>
#include <blaze/math/DynamicVector.h>
#include <blaze/util/Types.h>

#include <ieompp/types/matrix.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Scalar, bool StorageOrder>
        struct is_matrix<blaze::DynamicMatrix<Scalar, StorageOrder>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, bool StorageOrder>
        struct is_matrix<blaze::CompressedMatrix<Scalar, StorageOrder>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, bool TransposeFlag>
        struct is_vector<blaze::DynamicVector<Scalar, TransposeFlag>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, bool TransposeFlag>
        struct is_vector<blaze::CompressedVector<Scalar, TransposeFlag>> {
            static constexpr bool value = true;
        };

        template <typename Scalar>
        struct is_row_vector<blaze::DynamicVector<Scalar, blaze::rowVector>> {
            static constexpr bool value = true;
        };

        template <typename Scalar>
        struct is_row_vector<blaze::CompressedVector<Scalar, blaze::rowVector>> {
            static constexpr bool value = true;
        };

        template <typename Scalar>
        struct is_column_vector<blaze::DynamicVector<Scalar, blaze::columnVector>> {
            static constexpr bool value = true;
        };

        template <typename Scalar>
        struct is_column_vector<blaze::CompressedVector<Scalar, blaze::columnVector>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, bool StorageOrder>
        struct scalar_type<blaze::DynamicMatrix<Scalar, StorageOrder>> {
            using type = Scalar;
        };

        template <typename Scalar, bool StorageOrder>
        struct scalar_type<blaze::CompressedMatrix<Scalar, StorageOrder>> {
            using type = Scalar;
        };

        template <typename Scalar, bool TransposeFlag>
        struct scalar_type<blaze::DynamicVector<Scalar, TransposeFlag>> {
            using type = Scalar;
        };

        template <typename Scalar, bool TransposeFlag>
        struct scalar_type<blaze::CompressedVector<Scalar, TransposeFlag>> {
            using type = Scalar;
        };

        template <typename Scalar, bool StorageOrder>
        struct index_type<blaze::DynamicMatrix<Scalar, StorageOrder>> {
            using type = blaze::size_t;
        };

        template <typename Scalar, bool StorageOrder>
        struct index_type<blaze::CompressedMatrix<Scalar, StorageOrder>> {
            using type = blaze::size_t;
        };

        template <typename Scalar, bool TransposeFlag>
        struct index_type<blaze::DynamicVector<Scalar, TransposeFlag>> {
            using type = blaze::size_t;
        };

        template <typename Scalar, bool TransposeFlag>
        struct index_type<blaze::CompressedVector<Scalar, TransposeFlag>> {
            using type = blaze::size_t;
        };


        template <typename T>
        struct is_blaze_dense_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, bool StorageOrder>
        struct is_blaze_dense_matrix<blaze::DynamicMatrix<Scalar, StorageOrder>> {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_blaze_sparse_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, bool StorageOrder>
        struct is_blaze_sparse_matrix<blaze::CompressedMatrix<Scalar, StorageOrder>> {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_blaze_matrix {
            static constexpr bool value =
                is_blaze_dense_matrix<T>::value || is_blaze_sparse_matrix<T>::value;
        };

        template <typename Scalar, bool StorageOrder>
        struct matrix_dimension_info<blaze::DynamicMatrix<Scalar, StorageOrder>> {
            static blaze::size_t rows(const blaze::DynamicMatrix<Scalar, StorageOrder>& m)
            {
                return m.rows();
            }

            static blaze::size_t columns(const blaze::DynamicMatrix<Scalar, StorageOrder>& m)
            {
                return m.columns();
            }
        };

        template <typename Scalar, bool StorageOrder>
        struct matrix_dimension_info<blaze::CompressedMatrix<Scalar, StorageOrder>> {
            static blaze::size_t rows(const blaze::CompressedMatrix<Scalar, StorageOrder>& m)
            {
                return m.rows();
            }

            static blaze::size_t columns(const blaze::CompressedMatrix<Scalar, StorageOrder>& m)
            {
                return m.columns();
            }
        };

        template <typename Scalar, bool TransposeFlag>
        struct matrix_dimension_info<blaze::DynamicVector<Scalar, TransposeFlag>> {
            static blaze::size_t rows(const blaze::DynamicVector<Scalar, TransposeFlag>& v)
            {
                return v.size();
            }

            static blaze::size_t columns(const blaze::DynamicVector<Scalar, TransposeFlag>&)
            {
                return 1;
            }
        };

        template <typename Scalar, bool TransposeFlag>
        struct matrix_dimension_info<blaze::CompressedVector<Scalar, TransposeFlag>> {
            static blaze::size_t rows(const blaze::CompressedVector<Scalar, TransposeFlag>& v)
            {
                return v.size();
            }

            static blaze::size_t columns(const blaze::CompressedVector<Scalar, TransposeFlag>&)
            {
                return 1;
            }
        };
    }
}

#endif
