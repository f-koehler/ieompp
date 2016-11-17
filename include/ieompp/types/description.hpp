#ifndef IEOMPP_TYPES_DESCRIPTION_HPP_
#define IEOMPP_TYPES_DESCRIPTION_HPP_

#include "ieompp/description.hpp"
#include "ieompp/string.hpp"
#include "ieompp/types/names.hpp"

#include <complex>
#include <cstdint>
#include <string>
#include <tuple>

namespace ieompp
{

    template <>
    struct TypeDescription<bool> {
        static Description description()
        {
            return {{"type", "bool"}, {"size", std::to_string(sizeof(bool))}};
        }
    };

    template <>
    struct TypeDescription<uint8_t> {
        static Description description()
        {
            return {{"type", "uint8_t"}, {"size", std::to_string(sizeof(uint8_t))}};
        }
    };

    template <>
    struct TypeDescription<uint16_t> {
        static Description description()
        {
            return {{"type", "uint16_t"}, {"size", std::to_string(sizeof(uint16_t))}};
        }
    };

    template <>
    struct TypeDescription<uint32_t> {
        static Description description()
        {
            return {{"type", "uint32_t"}, {"size", std::to_string(sizeof(uint32_t))}};
        }
    };

    template <>
    struct TypeDescription<uint64_t> {
        static Description description()
        {
            return {{"type", "uint64_t"}, {"size", std::to_string(sizeof(uint64_t))}};
        }
    };

    template <>
    struct TypeDescription<int8_t> {
        static Description description()
        {
            return {{"type", "int8_t"}, {"size", std::to_string(sizeof(int8_t))}};
        }
    };

    template <>
    struct TypeDescription<int16_t> {
        static Description description()
        {
            return {{"type", "int16_t"}, {"size", std::to_string(sizeof(int16_t))}};
        }
    };

    template <>
    struct TypeDescription<int32_t> {
        static Description description()
        {
            return {{"type", "int32_t"}, {"size", std::to_string(sizeof(int32_t))}};
        }
    };

    template <>
    struct TypeDescription<int64_t> {
        static Description description()
        {
            return {{"type", "int64_t"}, {"size", std::to_string(sizeof(int64_t))}};
        }
    };

    template <>
    struct TypeDescription<float> {
        static Description description()
        {
            return {{"type", "float"}, {"size", std::to_string(sizeof(float))}};
        }
    };

    template <>
    struct TypeDescription<double> {
        static Description description()
        {
            return {{"type", "double"}, {"size", std::to_string(sizeof(double))}};
        }
    };

    template <>
    struct TypeDescription<long double> {
        static Description description()
        {
            return {{"type", "long double"}, {"size", std::to_string(sizeof(long double))}};
        }
    };

    template <typename Float>
    struct TypeDescription<std::complex<Float>> {
        static Description description()
        {
            return {{"type", compose("complex<", types::type_name<Float>::name, ">")}};
        }
    };
} // namespace ieompp

#endif
