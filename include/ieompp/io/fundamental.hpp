#ifndef IEOMPP_IO_FUNDAMENTAL_HPP_
#define IEOMPP_IO_FUNDAMENTAL_HPP_

#include <complex>
#include <istream>
#include <ostream>
#include <string>

namespace ieompp
{
    namespace io
    {
        template <typename OStream, typename T>
        inline typename std::enable_if<std::is_fundamental<T>::value, void>::type
        write_binary(OStream& strm, const T& t)
        {
            static constexpr auto size = sizeof(t);
            const char* data = (const char*)(&t);
            strm.write(data, size);
        }

        template <typename OStream, typename T>
        inline void write_binary(OStream& strm, const std::complex<T>& t)
        {
            write_binary<OStream, T>(strm, t.real());
            write_binary<OStream, T>(strm, t.imag());
        }


        template <typename IStream, typename T>
        inline typename std::enable_if<std::is_fundamental<T>::value, void>::type
        read_binary(IStream& strm, T& t)
        {
            static constexpr auto size = sizeof(t);
            char* data = (char*)(&t);
            strm.read(data, size);
        }

        template <typename IStream, typename T>
        inline void read_binary(IStream& strm, std::complex<T>& t)
        {
            T buf;
            read_binary<IStream, T>(strm, buf);
            t.real(buf);
            read_binary<IStream, T>(strm, buf);
            t.imag(buf);
        }
    }
}

#endif
