#ifndef IEOMPP_IO_LINE_HPP_
#define IEOMPP_IO_LINE_HPP_

#include <regex>

namespace ieompp
{
    namespace io
    {
        bool is_skippable_line(const std::string str)
        {
            static const std::regex reg("^\\s*(?:#.*)?$");
            return std::regex_match(str, reg);
        }
    }
}

#endif
