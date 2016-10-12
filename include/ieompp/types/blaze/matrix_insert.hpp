#ifndef IEOMPP_TYPES_BLAZE_MATRIX_INSERT_HPP_
#define IEOMPP_TYPES_BLAZE_MATRIX_INSERT_HPP_

#include <ieompp/types/blaze/traits.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename Scalar, bool StorageOrder, typename Index>
        void matrix_insert(blaze::CompressedMatrix<Scalar, StorageOrder>& matrix, Index row,
                           Index col, const Scalar& value)
        {
            matrix.insert(row, col, value);
        }
    }
}


#endif
