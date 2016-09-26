#ifndef IEOMPP_PLATFORM_HPP_
#define IEOMPP_PLATFORM_HPP_

#include <sstream>
#include <string>

#include <Eigen/src/Core/util/Macros.h>
#include <boost/predef.h>
#include <boost/version.hpp>

namespace ieompp
{
    namespace platform
    {
        template <typename... Ts>
        std::string stringize(const Ts&... ts)
        {
            std::ostringstream strm;
            using Tmp = int[];
            (void)Tmp{0, ((void)(strm << ts), 0)...};
            return strm.str();
        }

        auto boost()
        {
            static const auto str =
                stringize(int(BOOST_VERSION / 100000), '.', int(BOOST_VERSION / 100 % 1000), '.',
                          BOOST_VERSION % 100);
            return str;
        }

        auto eigen()
        {
            static const auto str =
                stringize(EIGEN_WORLD_VERSION, '.', EIGEN_MAJOR_VERSION, '.', EIGEN_MINOR_VERSION);
            return str;
        }

        auto architecture()
        {
#if BOOST_ARCH_X86_32
            static const auto str = stringize("x86_32");
#elif BOOST_ARCH_X86_64
            static const auto str = stringize("x86_64");
#else
            static const auto str = stringize("UntestedArch");
#endif
            return str;
        }

        auto operating_system()
        {
#if BOOST_OS_LINUX
            static const auto str = stringize("Linux");
#else
            static const auto str = stringize("UntestedOS");
#endif
            return str;
        }

        auto compiler()
        {
#if BOOST_COMP_CLANG
            static const auto str = stringize("clang ", __clang_major__, '.', __clang_minor__, '.',
                                              __clang_patchlevel__);
#elif BOOST_COMP_GNUC
            static const auto str =
                stringize("gcc ", __GNUC__, '.', __GNUC_MINOR__, '.', __GNUC_PATCHLEVEL__);
#else
            static const auto str = stringize("UntestedCompiler");
#endif
            return str;
        }

        auto cpp_library()
        {
#if defined(BOOST_LIB_STD_GNU)
            static const auto str = stringize("libstdc++", BOOST_LIB_STD_GNU);
#elif defined(BOOST_LIB_STD_CXX)
            static const auto str = stringize("libc++", _LIBCPP_VERSION);
#else
            static const auto str = "UnknownC++Library";
#endif
            return str;
        }

        enum class Endian : uint8_t { Unknown, Big, Little, BigWord, LittleWord };
        auto endianess()
        {
#if defined(BOOST_ENDIAN_BIG_BYTE)
            return Endian::Big;
#elif defined(BOOST_ENDIAN_LITTLE_BYTE)
            return Endian::Little;
#elif defined(BOOST_ENDIAN_BIG_WORD)
            return Endian::BigWord;
#elif defined(BOOST_LITTLE_WORD)
            return Endian::LittleWord;
#endif
        }

        std::ostream& operator<<(std::ostream& strm, Endian e)
        {
            switch(e) {
                case Endian::Big:
                    strm << "BigEndian";
                    return strm;
                case Endian::Little:
                    strm << "LittleEndian";
                    return strm;
                case Endian::BigWord:
                    strm << "BigWordSwappedEndian";
                    return strm;
                case Endian::LittleWord:
                    strm << "LittleWordSwappedEndian";
                    return strm;
                default:
                    strm << "UnknownEndian";
                    return strm;
            }
        }
    }
}


#if(BOOST_COMP_CLANG)
#pragma clang diagnostic pop
#endif

#endif
