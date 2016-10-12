#ifndef IEOMPP_TYPES_NAMES_HPP_
#define IEOMPP_TYPES_NAMES_HPP_

#include <complex>
#include <cstdint>

namespace ieompp
{
    namespace types
    {
        template <typename T>
        struct type_name {
        };

        template <>
        struct type_name<bool> {
            static constexpr auto name = "bool";
        };

        template <>
        struct type_name<uint8_t> {
            static constexpr auto name = "uint8_t";
        };

        template <>
        struct type_name<uint16_t> {
            static constexpr auto name = "uint16_t";
        };

        template <>
        struct type_name<uint32_t> {
            static constexpr auto name = "uint32_t";
        };

        template <>
        struct type_name<uint64_t> {
            static constexpr auto name = "uint64_t";
        };

        template <>
        struct type_name<int8_t> {
            static constexpr auto name = "int8_t";
        };

        template <>
        struct type_name<int16_t> {
            static constexpr auto name = "int16_t";
        };

        template <>
        struct type_name<int32_t> {
            static constexpr auto name = "int32_t";
        };

        template <>
        struct type_name<int64_t> {
            static constexpr auto name = "int64_t";
        };

        template <>
        struct type_name<float> {
            static constexpr auto name = "float";
        };

        template <>
        struct type_name<double> {
            static constexpr auto name = "double";
        };

        template <>
        struct type_name<long double> {
            static constexpr auto name = "long double";
        };
    }
}

#endif
