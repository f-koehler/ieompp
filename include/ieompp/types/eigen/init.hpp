#ifndef IEOMPP_TYPES_EIGEN_INIT_HPP_
#define IEOMPP_TYPES_EIGEN_INIT_HPP_

#include "ieompp/types/eigen/traits.hpp"

namespace ieompp
{
    namespace types
    {
        template <typename Matrix>
        typename std::enable_if<IsEigenSparseMatrix<Matrix>::value, void>::type
        init(Matrix& m, typename IndexType<Matrix>::Type rows,
             typename IndexType<Matrix>::index cols,
             std::function<typename ScalarType<Matrix>::Type(typename IndexType<Matrix>::Type,
                                                             typename IndexType<Matrix>::Type)>
                 f)
        {
            using Index = typename IndexType<Matrix>::Type;

            m = Matrix(rows, cols);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    const auto val = f(i, j);
                    if(!IsZero(val)) m.insert(i, j) = val;
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<IsEigenSparseMatrix<Matrix>::value, void>::type init_symmetric(
            Matrix& m, typename IndexType<Matrix>::Type rows,
            std::function<typename ScalarType<Matrix>::scalar(typename IndexType<Matrix>::Type,
                                                              typename IndexType<Matrix>::Type)>
                f)
        {
            using Index = typename IndexType<Matrix>::Type;

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
        typename std::enable_if<IsEigenDenseMatrix<Matrix>::value, void>::type
        init(Matrix& m, typename IndexType<Matrix>::Type rows,
             typename IndexType<Matrix>::Type cols,
             std::function<typename ScalarType<Matrix>::Type(typename IndexType<Matrix>::Type,
                                                             typename IndexType<Matrix>::Type)>
                 f)
        {
            using Index = typename IndexType<Matrix>::Index;

            m = Matrix::Zero(rows, cols);
            for(Index i = 0; i < rows; ++i) {
                for(Index j = 0; j < cols; ++j) {
                    m(i, j) = f(i, j);
                }
            }
        }

        template <typename Matrix>
        typename std::enable_if<IsEigenDenseMatrix<Matrix>::value, void>::type init_symmetric(
            Matrix& m, typename IndexType<Matrix>::Type rows,
            std::function<typename ScalarType<Matrix>::Type(typename IndexType<Matrix>::Type,
                                                            typename IndexType<Matrix>::Type)>
                f)
        {
            using Index = typename IndexType<Matrix>::Type;

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
