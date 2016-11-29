#ifndef SRC_HUBBARD_CHECKOINT_HPP_
#define SRC_HUBBARD_CHECKOINT_HPP_

#include <fstream>
#include <string>

#include <boost/filesystem.hpp>

#include <blaze/math/serialization/VectorSerializer.h>
#include <blaze/util/Serialization.h>

#include <ieompp/io/line.hpp>
#include <ieompp/types/blaze.hpp>

#include "logging.hpp"

template <typename Scalar, bool TransposeFlag, typename Time>
void write_checkpoint_file(const std::string& prefix,
                           const blaze::DynamicVector<Scalar, TransposeFlag>& vector, const Time& t)
{
    static uint64_t check_point_number = 0;
    std::string path                   = prefix + std::to_string(check_point_number) + ".blaze";
    get_loggers().io->info("Create checkpoint file \"{}\" using vector with {} elements", path,
                           vector.size());
    std::ofstream strm(path.c_str(), std::ofstream::binary);
    blaze::Archive<std::ofstream> archive(strm);
    archive << vector;
    strm.close();
    get_loggers().io->info("Finished writing checkpoint file \"{}\"", path);

    path = prefix + std::to_string(check_point_number) + ".info";
    get_loggers().io->info("Create checkpoint info file \"{}\" for t={}", path, t);
    strm.open(path.c_str());
    strm << t << '\n';
    strm.close();
    get_loggers().io->info("Finished writing checkpoint info file \"{}\"", path);

    ++check_point_number;
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
    get_loggers().io->info("Clean output file \"{}\" to contain {} entries", path, entries);
    get_loggers().io->info("Renam output file \"{}\" -> \"{}\"", path, path + ".bak");
    boost::filesystem::rename(path, path + ".bak");
    get_loggers().io->info("Finished renaming file");

    get_loggers().io->info("Open old output file \"{}\"", path);
    std::ifstream in_file((path + ".bak").c_str());

    get_loggers().io->info("Open new output \"{}\" file for writing", path);
    std::ofstream out_file(path);

    get_loggers().io->info("Finished opening both files");

    get_loggers().io->info("Start copying \"{}\" entries", entries);
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
    get_loggers().io->info("Finished copying entries {}", entries);

    get_loggers().io->info("Close files \"{}\" and \"{}\"", path, path + ".bak");
    out_file.close();
    in_file.close();
    get_loggers().io->info("Finished closing both files");
}

#endif
