#ifndef SRC_HUBBARD_CLI_HPP_
#define SRC_HUBBARD_CLI_HPP_

#include <fstream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "logging.hpp"

using OptionsDescription = boost::program_options::options_description;
using VariablesMap       = boost::program_options::variables_map;

template <typename T>
using Value = boost::program_options::typed_value<T>*;

void read_response_file(const std::string& path, VariablesMap& vm,
                        const OptionsDescription& description)
{
    get_loggers().main->info("Reading response file \"{}\"", path);
    std::ifstream file(path.c_str());
    std::string buf;
    std::vector<std::string> args;
    do {
        getline(file, buf);
        args.push_back(buf);
        boost::program_options::store(
            boost::program_options::command_line_parser(args).options(description).run(), vm);
    } while(!file.eof());
    file.close();
    get_loggers().main->info("Finish reading response file \"{}\"", path);
}

void write_response_file(const std::string& path, int argc, char** argv)
{
    get_loggers().io->info("Write response file \"{}\"", path);
    std::ofstream file(path.c_str());
    for(int i = 1; i < argc; ++i) {
        file << argv[i] << '\n';
    }
    file.close();
    get_loggers().io->info("Close response file \"{}\"", path);
}

#endif
