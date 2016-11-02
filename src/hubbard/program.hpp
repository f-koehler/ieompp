#ifndef SRC_PROGRAM_HPP_
#define SRC_PROGRAM_HPP_

#include <complex>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <blaze/math/serialization/MatrixSerializer.h>
#include <blaze/math/serialization/VectorSerializer.h>
#include <blaze/util/Serialization.h>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <ieompp/io/line.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
#include <ieompp/types/blaze.hpp>

struct Loggers {
    std::vector<spdlog::sink_ptr> sinks;
    std::shared_ptr<spdlog::logger> main, io, ode;

    Loggers(const std::string& log_path)
    {
        sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>(log_path));
        main = std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());
        io   = std::make_shared<spdlog::logger>("io", sinks.begin(), sinks.end());
        ode  = std::make_shared<spdlog::logger>("ode", sinks.begin(), sinks.end());
    }
};

void add_default_options(boost::program_options::options_description& description)
{
    // clang-format off
    description.add_options()
        ("help", "print this help message")
        ("version", "print version information")
        ("response_file", boost::program_options::value<std::string>(), "file to read program parameters from")
        ("checkpoint_interval", boost::program_options::value<std::uint64_t>()->default_value(1000), "steps between checkpoints")
        ("checkpoint", boost::program_options::value<std::string>(), "checkpoint to use for resume")
        ;
    // clang-format on
}

void write_platform_info(std::ostream& strm)
{
    strm << "# ieompp:     \t" << ieompp::version << '\n';
    strm << "# boost:      \t" << ieompp::Platform::boost() << '\n';
    strm << "# blaze:      \t" << ieompp::blaze_version() << '\n';
    strm << "# arch:       \t" << ieompp::Platform::architecture() << '\n';
    strm << "# os:         \t" << ieompp::Platform::operating_system() << '\n';
    strm << "# compiler:   \t" << ieompp::Platform::compiler() << '\n';
    strm << "# std lib:    \t" << ieompp::Platform::cpp_library() << '\n';
    strm << "# compiled by:\t" << ieompp::Platform::user() << '@' << ieompp::Platform::host() << '\n';
    strm << "# endianess   \t" << ieompp::Platform::endianess() << '\n';
}

void read_response_file(const std::string& path, boost::program_options::variables_map& vm,
                        boost::program_options::options_description& description)
{
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
}

void write_response_file(const std::string& path, int argc, char** argv, const Loggers& loggers)
{
    loggers.io->info("Write response file \"{}\"", path);
    std::ofstream file(path.c_str());
    for(int i = 1; i < argc; ++i) {
        file << argv[i] << '\n';
    }
    file.close();
    loggers.io->info("Close response file \"{}\"", path);
}

template <typename Scalar, bool StorageOrder>
void write_matrix_file(const std::string& path,
                       const blaze::CompressedMatrix<Scalar, StorageOrder>& matrix,
                       Loggers& loggers)
{
    loggers.io->info("Write matrix file \"{}\" using {}x{} matrix", path, matrix.rows(),
                     matrix.columns());
    std::ofstream strm(path.c_str(), std::ofstream::binary);
    blaze::Archive<std::ofstream> archive(strm);
    archive << matrix;
    strm.flush();
    strm.close();
}

template <typename Scalar, bool TransposeFlag>
void write_checkpoint_file(const std::string& path,
                           const blaze::DynamicVector<Scalar, TransposeFlag>& vector,
                           Loggers& loggers)
{
    loggers.io->info("Create checkpoint file \"{}\" using vector with {} elements", path,
                     vector.size());
    std::ofstream strm(path.c_str(), std::ofstream::binary);
    blaze::Archive<std::ofstream> archive(strm);
    archive << vector;
    strm.flush();
    strm.close();
}

template <typename Scalar, bool TransposeFlag>
void read_checkpoint_file(const std::string& path,
                          blaze::DynamicVector<Scalar, TransposeFlag>& vector, Loggers& loggers)
{
    loggers.io->info("Read checkpoint file \"{}\"", path);
    std::ifstream strm(path.c_str(), std::ifstream::binary);
    blaze::Archive<std::ifstream> archive(strm);
    archive >> vector;
    strm.close();
}

void clean_output_file(const std::string& path, uint64_t entries)
{
    boost::filesystem::rename(path, path + ".bak");

    std::ifstream in_file((path + ".bak").c_str());
    std::ofstream out_file(path);

    uint64_t counter = 0;
    std::string buffer;
    while(counter < entries) {
        std::getline(in_file, buffer);
        if(!ieompp::io::is_skippable_line(buffer)) {
            ++counter;
        }
        out_file << buffer;
    }
    out_file.flush();

    out_file.close();
    in_file.close();
}

#endif
