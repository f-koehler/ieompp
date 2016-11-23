#ifndef SRC_HUBBARD_APPLICATION_HPP_
#define SRC_HUBBARD_APPLICATION_HPP_

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/filesystem/convenience.hpp>

#include "application_timer.hpp"
#include "cli.hpp"
#include "logging.hpp"

struct Application {
    static std::string name;
    static std::string description;
    static OptionsDescription options_description;

    ApplicationTimer timer;
    VariablesMap variables;
    std::string output_path;
    std::string log_path;
    std::string response_path;
    std::string matrix_path;
    std::string checkpoint_prefix;
    std::ofstream output_file;

    Application(int argc, char** argv)
    {
        const auto all_options =
            parse_command_line_options(argc, argv, options_description, variables);

        if(variables.count("help")) {
            print_help();
            std::exit(1);
        }

        if(variables.count("version")) {
            print_version();
            std::exit(1);
        }

        output_path   = variables["out"].as<std::string>();
        response_path = boost::filesystem::change_extension(output_path, ".rsp").string();
        log_path      = boost::filesystem::change_extension(output_path, ".log").string();
        matrix_path =
            boost::filesystem::change_extension(output_path, "").string() + "_matrix.blaze";
        checkpoint_prefix =
            boost::filesystem::change_extension(output_path, "").string() + "_checkpoint_";

        get_loggers().init(log_path);

        get_loggers().main->info("CLI options:");
        for(const auto& option : all_options) {
            get_loggers().main->info("\t{} = {}", option.first, option.second);
        }

        if(variables.count("response_file") != 0u) {
            // read simulation parameters from response file
            read_response_file(variables["response_file"].as<std::string>(), variables,
                               Application::options_description);
        } else {
            // write a response file with simulation parameters
            get_loggers().io->info("Write response file \"{}\"", response_path);
            std::ofstream file(response_path.c_str());
            for(const auto& opt : all_options) {
                file << "--" << opt.first << '=' << opt.second << '\n';
            }
            file.close();
            get_loggers().io->info("Close response file \"{}\"", response_path);
        }

        get_loggers().io->info("Opening output file \"{}\"", output_path);
        output_file.open(output_path.c_str());
        get_loggers().io->info("File \"{}\" is now open", output_path);

        output_file << "# program     \t" << name << '\n';
        output_file << "# ieompp:     \t" << ieompp::version << '\n';
        output_file << "# boost:      \t" << ieompp::Platform::boost() << '\n';
        output_file << "# blaze:      \t" << ieompp::blaze_version() << '\n';
        output_file << "# arch:       \t" << ieompp::Platform::architecture() << '\n';
        output_file << "# os:         \t" << ieompp::Platform::operating_system() << '\n';
        output_file << "# compiler:   \t" << ieompp::Platform::compiler() << '\n';
        output_file << "# std lib:    \t" << ieompp::Platform::cpp_library() << '\n';
        output_file << "# compiled by:\t" << ieompp::Platform::user() << '@'
                    << ieompp::Platform::host() << '\n';
        output_file << "# endianess   \t" << ieompp::Platform::endianess() << '\n';

        output_file << "#\n# CLI options:\n";
        for(const auto& option : all_options) {
            output_file << "#\t" << option.first << " = " << option.second << '\n';
        }
    }

    Application(const Application&) = delete;

    ~Application()
    {
        get_loggers().io->info("Closing output file \"{}\"", output_path);
        output_file.close();
        get_loggers().io->info("File \"{}\" closed", output_path);

        get_loggers().main->info("Execution time: {}", timer);
    }

    static void add_default_options()
    {
        // clang-format off
        options_description.add_options()
            ("help", "print this help message")
            ("version", "print version information")
            ("response_file", make_value<std::string>(), "file to read program parameters from")
            ("checkpoint_interval", make_value<std::uint64_t>(1000), "steps between checkpoints")
            ("checkpoint", make_value<std::string>(), "checkpoint to use for resume")
            ("out", make_value<std::string>(name + ".txt"), "output file")
            ;
        // clang-format on
    }

    static void print_help()
    {
        std::cout << name << "\n\n";
        std::cout << "Usage:\n";
        std::cout << "  " << Application::name << " [options]" << '\n';
        std::cout << "  " << Application::name << " [options] --checkpoint=<path>" << '\n';
        std::cout << '\n';
        std::cout << Application::description << '\n';
        std::cout << Application::options_description << '\n';
    }

    static void print_version()
    {
        std::cout << "program:    \t" << name << '\n';
        std::cout << "ieompp:     \t" << ieompp::version << '\n';
        std::cout << "boost:      \t" << ieompp::Platform::boost() << '\n';
        std::cout << "blaze:      \t" << ieompp::blaze_version() << '\n';
        std::cout << "arch:       \t" << ieompp::Platform::architecture() << '\n';
        std::cout << "os:         \t" << ieompp::Platform::operating_system() << '\n';
        std::cout << "compiler:   \t" << ieompp::Platform::compiler() << '\n';
        std::cout << "std lib:    \t" << ieompp::Platform::cpp_library() << '\n';
        std::cout << "compiled by:\t" << ieompp::Platform::user() << '@' << ieompp::Platform::host()
                  << '\n';
        std::cout << "endianess   \t" << ieompp::Platform::endianess() << '\n';
    }
};

std::string Application::name        = "";
std::string Application::description = "";
OptionsDescription Application::options_description;

#endif
