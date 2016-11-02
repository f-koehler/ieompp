#ifndef IEOMPP_IO_LINE_HPP_
#define IEOMPP_IO_LINE_HPP_

#include <istream>
#include <regex>

namespace ieompp
{
    namespace io
    {
        bool is_skippable_line(const std::string& str)
        {
            static const std::regex reg(R"(^\s*(?:#.*)?$)");
            return std::regex_match(str, reg);
        }

        std::string next_line(std::istream& strm)
        {
            std::string buf;
            do {
                std::getline(strm, buf);
                if(!is_skippable_line(buf)) {
                    return buf;
                }
            } while(true);
        }
    } // namespace io
} // namespace ieompp

#endif
