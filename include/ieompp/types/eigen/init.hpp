#ifndef IEOMPP_TYPES_EIGEN_INIT_HPP_
#define IEOMPP_TYPES_EIGEN_INIT_HPP_

#include "ieompp/types/eigen/traits.hpp"

namespace ieompp
{
    namespace types
    {
        template <typename Matrix>
        typename std::enable_if<is_eigen_sparse_matrix<Matrix>::value, void>::type
        init(Matrix& m, typename index_type<Matrix>::type rows,
             typename index_type<Matrix>::index cols,
             std::function<typename scalar_type<Matrix>::type(typename index_type<Matrix>::type,
                                                              typename index_type<Matrix>::type)>
                 f)
        {
            using Index = typename index_type<Matrix>::type;

            m = Matrix(rows, cols);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    const auto val = f(i, j);
                    if(!IsZero(val)) m.insert(i, j) = val;
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_eigen_sparse_matrix<Matrix>::value, void>::type init_symmetric(
            Matrix& m, typename index_type<Matrix>::type rows,
            std::function<typename scalar_type<Matrix>::scalar(typename index_type<Matrix>::type,
                                                               typename index_type<Matrix>::type)>
                f)
        {
            using Index = typename index_type<Matrix>::type;

            m = Matrix(rows, rows);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < i; ++j) {
                    const auto val = f(i, j);
                    if(!IsZero(val)) {
                        m.insert(i, j) = val;
                        m.insert(j, i) = val;
                    }
                }
                const auto val = f(i, i);
                if(!IsZero(val)) m.insert(i, i) = val;
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_eigen_dense_matrix<Matrix>::value, void>::type
        init(Matrix& m, typename index_type<Matrix>::type rows,
             typename index_type<Matrix>::type cols,
             std::function<typename scalar_type<Matrix>::type(typename index_type<Matrix>::type,
                                                              typename index_type<Matrix>::type)>
                 f)
        {
            using Index = typename index_type<Matrix>::Index;

            m = Matrix::Zero(rows, cols);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    m(i, j) = f(i, j);
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<is_eigen_dense_matrix<Matrix>::value, void>::type init_symmetric(
            Matrix& m, typename index_type<Matrix>::type rows,
            std::function<typename scalar_type<Matrix>::type(typename index_type<Matrix>::type,
                                                             typename index_type<Matrix>::type)>
                f)
        {
            using Index = typename index_type<Matrix>::type;

            m = Matrix::Zero(rows, rows);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < i; ++j) {
                    const auto val = f(i, j);
                    m(i, j) = val;
                    m(j, i) = val;
                }
                m(i, i) = f(i, i);
            }
        }
    }
}

#endif
