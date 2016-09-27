#ifndef IEOMPP_IO_EIGEN_DENSE_HPP_
#define IEOMPP_IO_EIGEN_DENSE_HPP_

#include <fstream>

#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    namespace io
    {
        template <typename Matrix>
        typename std::enable_if<types::is_sparse_eigen_matrix<Matrix>::value, void>::type
        write_matrix(std::ostream& strm, const Matrix& m, bool binary = false)
        {
            using Index = typename types::eigen_matrix_traits<Matrix>::Index;

            if(!binary) {
                for(Index i = 0; i < m.outerSize(); ++i) {
                    for(typename Matrix::InnerIterator it(m, i); it; ++it) {
                        strm << '\t' << it.row() << '\t' << it.col() << '\t' << it.value() << '\n';
                    }
                }
            }
        }
    }
}

#endif
