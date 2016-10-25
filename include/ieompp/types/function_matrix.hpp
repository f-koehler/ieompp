#ifndef IEOMPP_TYPES_FUNCTION_MATRIX_HPP_
#define IEOMPP_TYPES_FUNCTION_MATRIX_HPP_

#include <functional>
#include <iomanip>
#include <ostream>

#include <ieompp/types/number.hpp>

namespace ieompp
{
    namespace types
    {
        template <typename ScalarT, typename IndexT = std::size_t>
        class FunctionMatrix
        {
        public:
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

            template <typename Matrix>
            bool operator==(const Matrix& rhs) const
            {
                if((_rows != rhs.rows()) || (_cols != rhs.cols())) {
                    return false;
                }
                for(Index row = 0; row < _rows; ++row) {
                    for(Index col = 0; col < _cols; ++col) {
                        if(!IsEqual(_f(row, col), rhs(row, col))) {
                            return false;
                        }
                    }
                }
                return true;
            }

            template <typename Matrix>
            bool operator!=(const Matrix& rhs) const
            {
                if((_rows != rhs.rows()) || (_cols != rhs.cols())) {
                    return true;
                }
                for(Index row = 0; row < _rows; ++row) {
                    for(Index col = 0; col < _cols; ++col) {
                        if(!IsEqual(_f(row, col), rhs(row, col))) {
                            return true;
                        }
                    }
                }
                return false;
            }
        };

        template <typename T>
        struct is_function_matrix {
            static constexpr bool value = false;
        };

        template <typename Scalar, typename Index>
        struct is_function_matrix<FunctionMatrix<Scalar, Index>> {
            static constexpr bool value = true;
        };

        template <typename Scalar, typename Index>
        std::ostream& operator<<(std::ostream& strm, const FunctionMatrix<Scalar, Index>& matrix)
        {
            strm << std::fixed << std::setprecision(1) << std::showpos;
            for(Index row = 0; row < matrix.rows(); ++row) {
                for(Index col = 0; col < matrix.cols(); ++col) {
                    strm << matrix(row, col) << ' ';
                }
                strm << '\n';
            }
            return strm;
        }

        template <typename Scalar, typename Index>
        bool is_quadratic(const FunctionMatrix<Scalar, Index>& matrix)
        {
            return matrix.rows() == matrix.cols();
        }

        template <typename Scalar, typename Index>
        bool is_symmetric(const FunctionMatrix<Scalar, Index>& matrix)
        {
            if(!is_quadratic(matrix)) {
                return false;
            }

            const Index N = matrix.rows();
            for(Index row = 0; row < N; ++row) {
                for(Index col = 0; col < row; ++col) {
                    if(!IsEqual(matrix(row, col), matrix(col, row))) {
                        return false;
                    }
                }
            }

            return true;
        }

        template <typename Scalar, typename Index>
        bool is_hermitian(const FunctionMatrix<Scalar, Index>& matrix)
        {
            if(!is_quadratic(matrix)) {
                return false;
            }

            const Index N = matrix.rows();
            for(Index row = 0; row < N; ++row) {
                for(Index col = 0; col <= row; ++col) {
                    if(!IsEqual(matrix(row, col), conjugate(matrix(col, row)))) {
                        return false;
                    }
                }
            }

            return true;
        }
    } // namespace types
} // namespace ieompp

#endif
