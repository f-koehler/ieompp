#ifndef IEOMPP_DOTPRODUCT_HPP_
#define IEOMPP_DOTPRODUCT_HPP_

#include <type_traits>

#include <Eigen/Dense>

namespace ieompp
{
    template <typename Vector>
    typename std::enable_if<std::is_floating_point<Vector>::value, Vector>::type
    dot_product(const Vector& a, const Vector& b)
    {
        return a * b;
    }

    // TODO: this currently assumes that the only other case is a vector from Eigen, not perfect!
    template <typename Vector>
    typename std::enable_if<!std::is_floating_point<Vector>::value, Vector>::type
    dot_product(const Vector& a, const Vector& b)
    {
        return a.dot(b);
    }
}

#endif
