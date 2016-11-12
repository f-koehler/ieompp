#ifndef SRC_HUBBARD_LOGGING_HPP_
#define SRC_HUBBARD_LOGGING_HPP_

#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
#include <ieompp/types/blaze.hpp>

#include <memory>
#include <vector>

struct Loggers {
    std::vector<spdlog::sink_ptr> sinks;
    std::shared_ptr<spdlog::logger> main, io, ode;

    void init(const std::string& log_path)
    {
        sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>(log_path));
        main = std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());
        io   = std::make_shared<spdlog::logger>("io", sinks.begin(), sinks.end());
        ode  = std::make_shared<spdlog::logger>("ode", sinks.begin(), sinks.end());

        main->info("Enable logging");
        main->info("");
        main->info("Platform:");
        main->info("\tieompp:     \t{}", ieompp::version);
        main->info("\tboost:      \t{}", ieompp::Platform::boost());
        main->info("\tblaze:      \t{}", ieompp::blaze_version());
        main->info("\tarch:       \t{}", ieompp::Platform::architecture());
        main->info("\tos:         \t{}", ieompp::Platform::operating_system());
        main->info("\tcompiler:   \t{}", ieompp::Platform::compiler());
        main->info("\tstd lib:    \t{}", ieompp::Platform::cpp_library());
        main->info("\tcompiled by:\t{}@{}", ieompp::Platform::user(), ieompp::Platform::host());
        main->info("\tendianess   \t{}", ieompp::Platform::endianess());
        main->info("");
    }
};

Loggers& get_loggers()
{
    static Loggers loggers;
    return loggers;
}

#endif
