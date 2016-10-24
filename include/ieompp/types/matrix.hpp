#ifndef IEOMPP_TYPES_MATRIX_HPP_
#define IEOMPP_TYPES_MATRIX_HPP_

namespace ieompp
{
    namespace types
    {
        template <typename T>
        struct is_matrix {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_vector {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_row_vector {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_column_vector {
            static constexpr bool value = false;
        };

        template <typename T>
        struct scalar_type {
        };

        template <typename T>
        struct index_type {
        };

        template <typename T>
        struct matrix_dimension_info {
        };
    }
}

#endif
