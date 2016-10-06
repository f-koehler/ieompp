#ifndef IEOMPP_SPDLOG_HPP_
#define IEOMPP_SPDLOG_HPP_

#include <spdlog/spdlog.h>

#include <ieompp/description.hpp>
#include <ieompp/string.hpp>

namespace ieompp
{
    enum class LogLevel : uint8_t { Debug, Info, Warning, Critical };
    template <typename Logger, LogLevel level = LogLevel::Info>
    void log(Logger logger, const Description& description){
        std::size_t longest = 0;
        for(auto& entry : description) {
            if(!entry.second.empty()) longest = std::max(longest, entry.second.size());
        }

        std::function<void(const std::string&)> log_f;
        switch(level) {
            case LogLevel::Debug:
                log_f = [&logger](const std::string& message) { logger->debug(message); };
                break;
            case LogLevel::Info:
                log_f = [&logger](const std::string& message) { logger->info(message); };
                break;
            case LogLevel::Warning:
                log_f = [&logger](const std::string& message) { logger->warn(message); };
                break;
            default:
                log_f = [&logger](const std::string& message) { logger->critical(message); };
        }

        for(auto& entry : description) {
            if(entry.second.empty()) {
                log_f(entry.first);
            } else {
                const auto size = entry.first.size();
                log_f(compose(entry.first,
                              (longest > size) ? std::string(longest - size, ' ') : std::string(""),
                              " = ", entry.second));
            }
        }
        log_f("");
    }
}

#endif
