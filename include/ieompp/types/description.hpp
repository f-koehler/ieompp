#ifndef IEOMPP_TYPES_DESCRIPTION_HPP_
#define IEOMPP_TYPES_DESCRIPTION_HPP_

#include <complex>
#include <cstdint>
#include <string>
#include <tuple>

#include <ieompp/description.hpp>
#include <ieompp/string.hpp>

namespace ieompp
{
    template <>
    struct TypeProperties<bool> {
        using type                 = bool;
        static constexpr auto name = "bool";
        static constexpr auto size = sizeof(bool);
    };

    template <>
    struct TypeProperties<int8_t> {
        using type                 = int8_t;
        static constexpr auto name = "int8_t";
        static constexpr auto size = sizeof(int8_t);
    };

    template <>
    struct TypeProperties<int16_t> {
        using type                 = int16_t;
        static constexpr auto name = "int16_t";
        static constexpr auto size = sizeof(int16_t);
    };

    template <>
    struct TypeProperties<int32_t> {
        using type                 = int32_t;
        static constexpr auto name = "int32_t";
        static constexpr auto size = sizeof(int32_t);
    };

    template <>
    struct TypeProperties<int64_t> {
        using type                 = int64_t;
        static constexpr auto name = "int64_t";
        static constexpr auto size = sizeof(int64_t);
    };

    template <>
    struct TypeProperties<uint8_t> {
        using type                 = uint8_t;
        static constexpr auto name = "uint8_t";
        static constexpr auto size = sizeof(uint8_t);
    };

    template <>
    struct TypeProperties<uint16_t> {
        using type                 = uint16_t;
        static constexpr auto name = "uint16_t";
        static constexpr auto size = sizeof(uint16_t);
    };

    template <>
    struct TypeProperties<uint32_t> {
        using type                 = uint32_t;
        static constexpr auto name = "uint32_t";
        static constexpr auto size = sizeof(uint32_t);
    };

    template <>
    struct TypeProperties<uint64_t> {
        using type                 = uint64_t;
        static constexpr auto name = "uint64_t";
        static constexpr auto size = sizeof(uint64_t);
    };
}

#endif
