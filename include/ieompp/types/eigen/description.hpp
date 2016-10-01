#ifndef IEOMPP_TYPES_EIGEN_DESCRIPTION_HPP_
#define IEOMPP_TYPES_EIGEN_DESCRIPTION_HPP_

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <ieompp/description.hpp>

namespace ieompp
{
    template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct TypeDescription<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Description get()
        {
            return {{"Dense matrix", ""},
                    {"  scalar", compose(TypeProperties<Scalar>::name, ' ', "(size ",
                                         TypeProperties<Scalar>::size, ")")}};
        }
    };

    template <typename Scalar, int Options, typename Index>
    struct TypeDescription<Eigen::SparseMatrix<Scalar, Options, Index>> {
        static Description get()
        {
            return {{"Sparse matrix", ""},
                    {"  scalar", compose(TypeProperties<Scalar>::name, ' ', "(size ",
                                         TypeProperties<Scalar>::size, ")")}};
        }
    };
}

#endif
