#ifndef IEOMPP_PLATFORM_HPP_
#define IEOMPP_PLATFORM_HPP_

#include "ieompp/openmp.hpp"
#include "ieompp/string.hpp"
#include "ieompp/version.hpp"

#include <cstdlib>

#include <boost/predef.h>
#include <boost/version.hpp>

#if BOOST_OS_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace ieompp
{
    struct Platform {
        static auto boost()
        {
            static const auto str =
                compose(int(BOOST_VERSION / 100000), '.', int(BOOST_VERSION / 100 % 1000), '.',
                        BOOST_VERSION % 100);
            return str;
        }

        static auto architecture()
        {
#if BOOST_ARCH_X86_32
            static const auto str = compose("x86_32");
#elif BOOST_ARCH_X86_64
            static const auto str = compose("x86_64");
#else
            static const auto str = compose("UntestedArch");
#endif
            return str;
        }

        static auto operating_system()
        {
#if BOOST_OS_LINUX
            static const auto str = compose("Linux");
#else
            static const auto str = compose("UntestedOS");
#endif
            return str;
        }

        static auto compiler()
        {
#if BOOST_COMP_CLANG
            static const auto str =
                compose("clang ", __clang_major__, '.', __clang_minor__, '.', __clang_patchlevel__);
#elif BOOST_COMP_GNUC
            static const auto str =
                compose("gcc ", __GNUC__, '.', __GNUC_MINOR__, '.', __GNUC_PATCHLEVEL__);
// BOOST_COMP_INTEL is bogus
#elif __INTEL_COMPILER
            static const auto str =
                compose("intel ", static_cast<uint16_t>(__INTEL_COMPILER / 100), '.',
                        __INTEL_COMPILER % 100, '.', __INTEL_COMPILER % 10);
#else
            static const auto str = compose("UntestedCompiler");
#endif
            return str;
        }

        static auto cpp_library()
        {
#if BOOST_LIB_STD_GNU
            static const auto str = compose("libstdc++", BOOST_LIB_STD_GNU);
#elif BOOST_LIB_STD_CXX
            static const auto str = compose("libc++", _LIBCPP_VERSION);
#else
            static const auto str = "UnknownC++Library";
#endif
            return str;
        }

        static auto host()
        {
#if BOOST_OS_WINDOWS
#else
            static char buffer[128];
            gethostname((char*)buffer, 128);
            return std::string((char*)buffer);
#endif
        }

        static auto user()
        {
#if BOOST_OS_WINDOWS
#else
            return std::string(std::getenv("LOGNAME"));
#endif
        }

        enum class Endian : uint8_t { Unknown, Big, Little, BigWord, LittleWord };
        static auto endianess()
        {
#if BOOST_ENDIAN_BIG_BYTE
            return Endian::Big;
#elif BOOST_ENDIAN_LITTLE_BYTE
            return Endian::Little;
#elif BOOST_ENDIAN_BIG_WORD
            return Endian::BigWord;
#elif BOOST_LITTLE_WORD
            return Endian::LittleWord;
#else
            return Endian::Unkown;
#endif
        }

        static auto max_threads() { return compose(omp_get_max_threads()); }
    };

    std::ostream& operator<<(std::ostream& strm, Platform::Endian e)
    {
        switch(e) {
            case Platform::Endian::Big:
                strm << "BigEndian";
                return strm;
            case Platform::Endian::Little:
                strm << "LittleEndian";
                return strm;
            case Platform::Endian::BigWord:
                strm << "BigWordSwappedEndian";
                return strm;
            case Platform::Endian::LittleWord:
                strm << "LittleWordSwappedEndian";
                return strm;
            default:
                strm << "UnknownEndian";
                return strm;
        }
    }
} // namespace ieompp

#endif
