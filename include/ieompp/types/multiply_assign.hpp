#ifndef IEOMPP_TYPES_MULTIPLY_ASSIGN_HPP_
#define IEOMPP_TYPES_MULTIPLY_ASSIGN_HPP_

namespace ieompp
{
    namespace types
    {
        template <typename Matrix, typename Vector>
        void multiply_assign(const Matrix& matrix, Vector& vector)
        {
            vector = matrix * vector;
        }
    } // namespace types
} // namespace ieompp

#endif
