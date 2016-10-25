#ifndef IEOMPP_STRING_HPP_
#define IEOMPP_STRING_HPP_

#include <sstream>
#include <string>

namespace ieompp
{
    template <typename... Ts>
    std::string compose(Ts... ts)
    {
        std::ostringstream strm;
        int _[] = {0, (strm << ts, 0)...};
        (void)_;
        return strm.str();
    }
} // namespace ieompp

#endif
