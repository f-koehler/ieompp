#ifndef SRC_HUBBARD_CHECKOINT_HPP_
#define SRC_HUBBARD_CHECKOINT_HPP_

#include <fstream>
#include <string>

#include <blaze/math/serialization/VectorSerializer.h>
#include <blaze/util/Serialization.h>

#include <ieompp/io/line.hpp>
#include <ieompp/types/blaze.hpp>

#include "logging.hpp"

template <typename Scalar, bool TransposeFlag>
void write_checkpoint_file(const std::string& path,
                           const blaze::DynamicVector<Scalar, TransposeFlag>& vector)
{
    get_loggers().io->info("Create checkpoint file \"{}\" using vector with {} elements", path,
                           vector.size());
    std::ofstream strm(path.c_str(), std::ofstream::binary);
    blaze::Archive<std::ofstream> archive(strm);
    archive << vector;
    strm.flush();
    strm.close();
    get_loggers().io->info("Finished writing checkpoint file \"{}\"");
}

template <typename Scalar, bool TransposeFlag>
void read_checkpoint_file(const std::string& path,
                          blaze::DynamicVector<Scalar, TransposeFlag>& vector)
{
    get_loggers().io->info("Read checkpoint file \"{}\"", path);
    std::ifstream strm(path.c_str(), std::ifstream::binary);
    blaze::Archive<std::ifstream> archive(strm);
    archive >> vector;
    strm.close();
    get_loggers().io->info("Finished reading checkpoint file \"{}\"", path);
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
