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

#include <boost/program_options.hpp>

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
        ;
    // clang-format on
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
    blaze::Archive<std::ofstream> archive(path);
    archive << vector;
    strm.flush();
    strm.close();
}

#endif
