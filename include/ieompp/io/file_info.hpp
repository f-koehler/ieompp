#ifndef IEOMPP_IO_FILE_INFO_HPP_
#define IEOMPP_IO_FILE_INFO_HPP_

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <ieompp/platform.hpp>
#include <ieompp/version.hpp>

namespace ieompp
{
    namespace io
    {
        class FileInfoHeader
        {
            public:
                using Entry = std::pair<std::string, std::string>;

                friend std::ostream& operator<<(std::ostream&, const FileInfoHeader&);

            private:
                std::vector<Entry> _entries;
                std::string _prefix;

            public:
                FileInfoHeader(bool default_entries = true, std::string prefix = "# ") : _prefix(prefix)
                {
                    if(default_entries) {
                        add_entry("Platform");
                        add_entry("  ieompp", ieompp::version);
                        add_entry("  boost", ieompp::platform::boost());
                        add_entry("  Compiler", ieompp::platform::compiler());
                        add_entry("  C++ std lib", ieompp::platform::cpp_library());
                        add_entry("  OS", ieompp::platform::operating_system());
                        add_entry("  Architecture", ieompp::platform::architecture());
                        add_entry("  Endian", ieompp::platform::endianess());
                    }
                }

                FileInfoHeader& add_entry(const std::string& key)
                {
                    _entries.emplace_back(key, "");
                    return *this;
                }

                template <typename T>
                FileInfoHeader& add_entry(const std::string& key, const T& val)
                {
                    std::ostringstream strm;
                    strm << val;
                    _entries.emplace_back(key, strm.str());
                    return *this;
                }

                FileInfoHeader& add_blank()
                {
                    add_entry("", "");
                    return* this;
                }
        };

        std::ostream& operator<<(std::ostream& strm, const FileInfoHeader& fi)
        {
            std::size_t max_key_len = 0;

            for(const auto& e : fi._entries) {
                if(e.second.empty()) continue;
                max_key_len = std::max(max_key_len, e.first.size());
            }

            const auto time =
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            strm << "# Created:";
            for(size_t i = 7; i <= max_key_len; ++i) strm << ' ';
            strm << std::put_time(std::gmtime(&time), "%Y-%m-%d:%H-%M-%S (%z)\n");
            if(fi._entries.empty()) return strm;

            strm << "#\n";
            for(const auto& e : fi._entries) {
                strm << fi._prefix << e.first;
                if(!e.second.empty()) {
                    strm << ':';
                    for(size_t i = e.first.size(); i <= max_key_len; ++i) strm << ' ';
                    strm << e.second;
                }
                strm << '\n';
            }

            return strm;
        }
    }
}

#endif
