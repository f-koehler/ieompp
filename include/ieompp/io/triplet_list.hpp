#ifndef IEOMPP_IO_TRIPLET_LIST_HPP_
#define IEOMPP_IO_TRIPLET_LIST_HPP_

#include "ieompp/io/line.hpp"
#include "ieompp/types/eigen/triplet_list.hpp"

#include <istream>
#include <ostream>
#include <string>

namespace ieompp
{
    namespace io
    {
        template <typename Scalar, typename Index = std::uint64_t>
        void write_triplet_list(std::ostream& strm, const types::TripletList<Scalar, Index>& list,
                                bool binary = false)
        {
            if(!binary) {
                strm << list.size() << '\n';
                for(auto& element : list) {
                    strm << element.row << '\t' << element.column << '\t' << element.value << '\n';
                }
            }
        }

        template <typename Scalar, typename Index = std::uint64_t>
        void read_triplet_list(std::istream& strm, types::TripletList<Scalar, Index>& list,
                               bool binary = false)
        {
            (void)list;
            if(!binary) {
                std::string buf;
                do {
                    std::getline(strm, buf);
                    if(is_skippable_line(buf)) {
                        continue;
                    }
                } while(!strm.eof());
            }
        }
    } // namespace io
} // namespace ieompp

#endif
