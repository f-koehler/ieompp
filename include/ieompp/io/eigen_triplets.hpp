#ifndef IEOMPP_IO_EIGEN_TRIPLETS_HPP_
#define IEOMPP_IO_EIGEN_TRIPLETS_HPP_

#include <ostream>
#include <istream>
#include <string>

#include <ieompp/io/line.hpp>
#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    namespace io
    {
        template <typename Container>
        typename std::enable_if<types::is_eigen_triplet<typename Container::value_type>::value,
                                void>::type
        write_triplet_list(std::ostream& strm, const Container& container, bool binary = false)
        {
            if(!binary) {
                strm << container.size() << '\n';
                for(auto& element : container) {
                    strm << element.row() << '\t' << element.col() << '\t' << element.value()
                         << '\n';
                }
            }
        }

        template <typename Container>
        typename std::enable_if<types::is_eigen_triplet<typename Container::value_type>::value,
                                void>::type
        read_triplet_list(std::istream& strm, const Container& container, bool binary = false)
        {
            if(!binary) {
                std::string buf;
                do {
                    std::getline(strm, buf);
                    if(is_skippable_line(buf)) continue;
                } while(!strm.eof());
            }
        }
    }
}

#endif
