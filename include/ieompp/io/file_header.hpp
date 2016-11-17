#ifndef IEOMPP_IO_FILE_HEADER_HPP_
#define IEOMPP_IO_FILE_HEADER_HPP_

#include "ieompp/description.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>

namespace ieompp
{
    namespace io
    {
        void write_header(std::ostream& strm, const std::vector<Description>& descriptions,
                          bool binary = false, const std::string prefix = "#")
        {
            if(!binary) {
                std::size_t max_key_len = 0;
                for(auto& description : descriptions) {
                    for(const auto& entry : description) {
                        if(entry.second.empty()) {
                            continue;
                        }
                        max_key_len = std::max(max_key_len, entry.first.size());
                    }
                }

                const auto time =
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                strm << prefix << " Created:";
                for(size_t i = 7; i <= max_key_len; ++i) {
                    strm << ' ';
                }
                strm << std::put_time(std::gmtime(&time), "%Y-%m-%d:%H-%M-%S (%z)\n");

                for(auto& description : descriptions) {
                    if(description.empty()) {
                        return;
                    }
                    strm << "#\n";
                    for(auto& entry : description) {
                        strm << prefix;
                        if(entry.first.empty()) {
                            strm << '\n';
                            continue;
                        }
                        strm << ' ' << entry.first;
                        if(entry.second.empty()) {
                            strm << '\n';
                            continue;
                        }
                        strm << ':';
                        for(size_t i = entry.first.size(); i <= max_key_len; ++i) {
                            strm << ' ';
                        }
                        strm << entry.second << '\n';
                    }
                }
            }
        }
    } // namespace io
} // namespace ieompp

#endif
