#ifndef SRC_HUBBARD_CLI_HPP_
#define SRC_HUBBARD_CLI_HPP_

#include <fstream>
#include <regex>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "logging.hpp"

using OptionsDescription = boost::program_options::options_description;
using VariablesMap       = boost::program_options::variables_map;

template <typename T>
auto make_value()
{
    return boost::program_options::value<T>();
}

template <typename T>
auto make_value(const T& default_value)
{
    return boost::program_options::value<T>()->default_value(default_value);
}

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

std::vector<std::pair<std::string, std::string>>
parse_command_line_options(int argc, char** argv,
                           const boost::program_options::options_description& description,
                           boost::program_options::variables_map& vm)
{
    const auto options = boost::program_options::parse_command_line(argc, argv, description);
    boost::program_options::store(options, vm);
    boost::program_options::notify(vm);

    std::vector<std::pair<std::string, std::string>> all_options;

    for(const auto& option : options.options) {
        if(option.unregistered) continue;
        for(const auto& value : option.value) {
            all_options.push_back(std::make_pair(option.string_key, value));
        }
    }

    static const auto regex = std::regex("^arg\\s+\\(=(.+)\\)$");
    for(const auto& option : description.options()) {
        const auto key = option->key("long name");
        if(std::find_if(options.options.begin(), options.options.end(),
                        [&key](const boost::program_options::basic_option<char>& opt) {
                            return (opt.string_key == key);
                        })
           != options.options.end()) {
            continue;
        }

        std::smatch m;
        const std::string name = option->semantic()->name();
        std::regex_match(name, m, regex);
        if(!m.empty()) {
            all_options.push_back(std::make_pair(key, m[1]));
        }
    }

    return all_options;
}

#endif
