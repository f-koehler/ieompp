#ifndef IEOMPP_IO_EIGEN_TRIPLETS_HPP_
#define IEOMPP_IO_EIGEN_TRIPLETS_HPP_

#include <ostream>

#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    namespace io
    {
        template <typename Container>
        typename std::enable_if<types::is_eigen_triplet<typename Container::value_type>::value,
                                void>::type
        write_matrix(std::ostream& strm, const Container& container, bool binary = false)
        {
            if(!binary) {
                strm << container.size() << '\n';
                for(auto& element : container) {
                    strm << element.row() << '\t' << element.col() << '\t' << element.value()
                         << '\n';
                }
            }
        }
    }
}

#endif
