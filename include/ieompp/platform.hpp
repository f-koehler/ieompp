#ifndef IEOMPP_PLATFORM_HPP_
#define IEOMPP_PLATFORM_HPP_

#include <cstdlib>
#include <sstream>
#include <string>

#include <Eigen/src/Core/util/Macros.h>
#include <boost/predef.h>
#include <boost/version.hpp>

#if BOOST_OS_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <ieompp/description.hpp>
#include <ieompp/version.hpp>

namespace ieompp
{
    template <typename... Ts>
    std::string stringize(const Ts&... ts)
    {
        std::ostringstream strm;
        using Tmp = int[];
        (void)Tmp{0, ((void)(strm << ts), 0)...};
        return strm.str();
    }

    struct Platform {
        auto boost() const
        {
            static const auto str =
                stringize(int(BOOST_VERSION / 100000), '.', int(BOOST_VERSION / 100 % 1000), '.',
                          BOOST_VERSION % 100);
            return str;
        }

        auto eigen() const
        {
            static const auto str =
                stringize(EIGEN_WORLD_VERSION, '.', EIGEN_MAJOR_VERSION, '.', EIGEN_MINOR_VERSION);
            return str;
        }

        auto architecture() const
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

        auto operating_system() const
        {
#if BOOST_OS_LINUX
            static const auto str = stringize("Linux");
#else
            static const auto str = stringize("UntestedOS");
#endif
            return str;
        }

        auto compiler() const
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

        auto cpp_library() const
        {
#if BOOST_LIB_STD_GNU
            static const auto str = stringize("libstdc++", BOOST_LIB_STD_GNU);
#elif defined(BOOST_LIB_STD_CXX)
            static const auto str = stringize("libc++", _LIBCPP_VERSION);
#else
            static const auto str = "UnknownC++Library";
#endif
            return str;
        }

        auto host() const
        {
#if BOOST_OS_WINDOWS
#else
            static char buffer[128];
            gethostname(buffer, 128);
            return std::string(buffer);
#endif
        }

        auto user() const
        {
#if BOOST_OS_WINDOWS
#else
            return std::string(std::getenv("LOGNAME"));
#endif
        }


        enum class Endian : uint8_t { Unknown, Big, Little, BigWord, LittleWord };
        auto endianess() const
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

    template <>
    struct VariableDescription<Platform> {
        static Description get(const Platform& platform)
        {
            return {{"Platform", ""},
                    {"  ieompp", version},
                    {"  boost", platform.boost()},
                    {"  Compiler", platform.compiler()},
                    {"  C++ std lib", platform.cpp_library()},
                    {"  OS", platform.operating_system()},
                    {"  Compiled by", stringize(platform.user(), '@', platform.host())},
                    {"  Architecture", platform.architecture()},
                    {"  Endian", stringize(platform.endianess())}};
        }
    };
}


#if(BOOST_COMP_CLANG)
#pragma clang diagnostic pop
#endif

#endif
