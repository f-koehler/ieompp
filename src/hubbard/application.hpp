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
        boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, options_description), variables);
        boost::program_options::notify(variables);

        if(variables.count("--help") != 0u) {
            print_help();
            std::exit(1);
        }

        if(variables.count("--version") != 0u) {
            print_version();
            std::exit(1);
        }

        output_path       = variables["out"].as<std::string>();
        response_path     = boost::filesystem::change_extension(output_path, ".rsp").string();
        log_path          = boost::filesystem::change_extension(output_path, ".log").string();
        matrix_path       = boost::filesystem::change_extension(output_path, "").string() + "_matrix.blaze";
        checkpoint_prefix = boost::filesystem::change_extension(output_path, "").string() + "_checkpoint_";

        get_loggers().init(log_path);

        if(variables.count("response_file") != 0u) {
            read_response_file(variables["response_file"].as<std::string>(), variables,
                               Application::options_description);
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
        output_file << "# compiled by:\t" << ieompp::Platform::user() << '@' << ieompp::Platform::host() << '\n';
        output_file << "# endianess   \t" << ieompp::Platform::endianess() << '\n';
    }

    Application(const Application&) = delete;

    ~Application() {
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
            ("response_file", boost::program_options::value<std::string>(), "file to read program parameters from")
            ("checkpoint_interval", boost::program_options::value<std::uint64_t>()->default_value(1000), "steps between checkpoints")
            ("checkpoint", boost::program_options::value<std::string>(), "checkpoint to use for resume")
            ("out", boost::program_options::value<std::string>()->default_value(name + ".txt"), "output file")
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

    static void print_version() {
        std::cout << "ieompp:     \t" << ieompp::version << '\n';
        std::cout << "boost:      \t" << ieompp::Platform::boost() << '\n';
        std::cout << "blaze:      \t" << ieompp::blaze_version() << '\n';
        std::cout << "arch:       \t" << ieompp::Platform::architecture() << '\n';
        std::cout << "os:         \t" << ieompp::Platform::operating_system() << '\n';
        std::cout << "compiler:   \t" << ieompp::Platform::compiler() << '\n';
        std::cout << "std lib:    \t" << ieompp::Platform::cpp_library() << '\n';
        std::cout << "compiled by:\t" << ieompp::Platform::user() << '@' << ieompp::Platform::host() << '\n';
        std::cout << "endianess   \t" << ieompp::Platform::endianess() << '\n';
    }
};

std::string Application::name        = "";
std::string Application::description = "";
OptionsDescription Application::options_description;

#endif
