#ifndef IEOMPP_IO_BINARY_HPP_
#define IEOMPP_IO_BINARY_HPP_

#include <complex>
#include <cstdint>
#include <istream>
#include <ostream>

namespace ieompp
{
    namespace io
    {
        template <typename T, std::size_t Size = sizeof(T)>
        union BinaryWrapper {
            T t;
            uint8_t data[Size];
        };

        template <typename T, std::size_t Size>
        std::ostream& operator<<(std::ostream& strm, const BinaryWrapper<T, Size>& wrapper)
        {
            strm.write(wrapper.data, Size);
        }

        template <typename T, std::size_t Size>
        std::istream& operator>>(std::istream& strm, BinaryWrapper<T, Size>& wrapper)
        {
            strm.read(wrapper.data, Size);
        }
    }
}

#endif
