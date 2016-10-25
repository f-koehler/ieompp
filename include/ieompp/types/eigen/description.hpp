#ifndef IEOMPP_TYPES_EIGEN_DESCRIPTION_HPP_
#define IEOMPP_TYPES_EIGEN_DESCRIPTION_HPP_

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include "ieompp/description.hpp"
#include "ieompp/string.hpp"

namespace ieompp
{
    template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct TypeDescription<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Description description()
        {
            return Description{{"type", "Eigen::Matrix"}, {"scalar", ""}}
                   + get_type_description<Scalar>().indent(2);
        }
    };

    template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    struct InstanceDescription<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> {
        static Description
        description(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& matrix)
        {
            return Description{
                {"rows",
                 compose(matrix.rows(), (Rows == Eigen::Dynamic) ? " (dynamic)" : " (fixed)")},
                {"cols",
                 compose(matrix.cols(), (Cols == Eigen::Dynamic) ? " (dynamic)" : " (fixed)")}};
        }
    };

    template <typename Scalar, int Options, typename Index>
    struct TypeDescription<Eigen::SparseMatrix<Scalar, Options, Index>> {
        static Description description()
        {
            return Description{{"type", "Eigen::SparseMatrix"}, {"scalar", ""}}
                   + get_type_description<Scalar>().indent(2) + Description{{"index", ""}}
                   + get_type_description<Index>().indent(2);
        }
    };

    template <typename Scalar, int Options, typename Index>
    struct InstanceDescription<Eigen::SparseMatrix<Scalar, Options, Index>> {
        static Description description(const Eigen::SparseMatrix<Scalar, Options, Index>& matrix)
        {
            return Description{{"rows", std::to_string(matrix.rows())},
                               {"cols", std::to_string(matrix.cols())}};
        }
    };
}

#endif
