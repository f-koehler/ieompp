#ifndef IEOMPP_IO_FILE_INFO_HPP_
#define IEOMPP_IO_FILE_INFO_HPP_

#include <algorithm>
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
        class FileInfo
        {
            public:
                using Entry = std::pair<std::string, std::string>;

                friend std::ostream& operator<<(std::ostream&, const FileInfo&);

            private:
                std::vector<Entry> _entries;
                std::string _prefix;

            public:
                FileInfo(bool default_entries = true, std::string prefix = "# ") : _prefix(prefix)
                {
                    if(default_entries) {
                        add_entry("ieompp", ieompp::version);
                        add_entry("boost", ieompp::platform::boost());
                        add_entry("Compiler", ieompp::platform::compiler());
                        add_entry("OS", ieompp::platform::operating_system());
                        add_entry("Architecture", ieompp::platform::architecture());
                    }
                }

                template <typename T>
                void add_entry(const std::string& key, const T& val)
                {
                    std::ostringstream strm;
                    strm << val;
                    _entries.emplace_back(key, strm.str());
                }
        };

        std::ostream& operator<<(std::ostream& strm, const FileInfo& fi) {
            std::size_t max_key_len = 0;
            for(const auto& e : fi._entries) max_key_len = std::max(max_key_len, e.first.size());

            for(const auto& e : fi._entries) {
                strm << fi._prefix << e.first << ':';
                for(size_t i = e.first.size(); i <= max_key_len; ++i) strm << ' ';
                strm << e.second << '\n';
            }

            return strm;
        }
    }
}

#endif
