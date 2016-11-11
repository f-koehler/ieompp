#ifndef IEOMPP_TYPES_MATRIX_HPP_
#define IEOMPP_TYPES_MATRIX_HPP_

namespace ieompp
{
    namespace types
    {
        template <typename T>
        struct IsMatrix {
            static constexpr bool value = false;
        };

        template <typename T>
        struct IsVector {
            static constexpr bool value = false;
        };

        template <typename T>
        struct IsRowVector {
            static constexpr bool value = false;
        };

        template <typename T>
        struct IsColumnVector {
            static constexpr bool value = false;
        };

        template <typename T>
        struct ScalarType {
            using Type = T;
        };

        template <typename T>
        struct IndexType {
        };

        template <typename T>
        struct MatrixDimensionInfo {
        };
    } // namespace types
} // namespace ieompp

#endif
