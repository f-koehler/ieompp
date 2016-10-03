#ifndef IEOMPP_TYPES_EIGEN_FUNCTION_MATRIX_HPP_
#define IEOMPP_TYPES_EIGEN_FUNCTION_MATRIX_HPP_

#include <functional>

namespace ieompp
{
    namespace types
    {
        template <typename ScalarT, typename IndexT = std::size_t>
        class FunctionMatrix
        {
            using Scalar = ScalarT;
            using Index  = IndexT;

        private:
            Index _rows, _cols;
            std::function<Scalar(Index, Index)> _f;

        public:
            FunctionMatrix(Index rows, Index cols, std::function<Scalar(Index, Index)> f)
                : _rows(rows), _cols(cols), _f(f)
            {
            }

            Index rows() const { return _rows; }
            Index cols() const { return _cols; }

            Scalar operator()(Index row, Index col) const { return _f(row, col); }

            template <typename Vector>
            Vector operator*(const Vector& vec) const
            {
                assert(vec.rows() == rows());

                Vector result = vec;
                for(Index row = 0; row < rows(); ++row) {
                    result(row) = 0;
                    for(Index col = 0; col < cols(); ++col) {
                        result(row) += _f(row, col) * vec(col);
                    }
                }
                return result;
            }
        };
    }
}

#endif
