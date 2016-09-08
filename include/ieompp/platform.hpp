#ifndef IEOMPP_PLATFORM_HPP_
#define IEOMPP_PLATFORM_HPP_

#include <string>
#include <sstream>

#include <boost/predef.h>
#include <boost/version.hpp>
#include <Eigen/src/Core/util/Macros.h>

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

    const auto IEOMPP_BOOST_VERSION =
        stringize(int(BOOST_VERSION / 100000), '.', int(BOOST_VERSION / 100 % 1000), '.',
                  BOOST_VERSION % 100);
    const auto IEOMPP_EIGEN_VERSION =
        stringize(EIGEN_WORLD_VERSION, '.', EIGEN_MAJOR_VERSION, '.', EIGEN_MINOR_VERSION);

#if BOOST_ARCH_X86_32
    const auto IEOMPP_ARCH = stringize("x86_32");
#elif BOOST_ARCH_X86_64
    const auto IEOMPP_ARCH = stringize("x86_64");
#else
    const auto IEOMPP_ARCH = stringize("UntestedArch");
#endif

#if BOOST_OS_LINUX
    const auto IEOMPP_OS = stringize("Linux");
#else
    const auto IEOMPP_OS = stringize("UntestedOS");
#endif

#if BOOST_COMP_CLANG
    const auto IEOMPP_COMPILER_NAME    = stringize("Clang");
    const auto IEOMPP_COMPILER_VERSION =
        stringize(__clang_major__, '.', __clang_minor__, '.', __clang_patchlevel__);
#elif BOOST_COMP_GNUC
    const auto IEOMPP_COMPILER_NAME    = stringize("Gcc");
    const auto IEOMPP_COMPILER_VERSION =
        stringize(__GNUC__, '.', __GNUC_MINOR__, '.', __GNUC_PATCHLEVEL__);
#else
    const auto IEOMPP_COMPILER_NAME    = stringize("UntestedCompiler");
    const auto IEOMPP_COMPILER_VERSION = stringize("UntestedVersion");
#endif
}

#endif
