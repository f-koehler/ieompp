#ifndef IEOMPP_STRING_HPP_
#define IEOMPP_STRING_HPP_

#include <sstream>
#include <string>

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

}

#endif
