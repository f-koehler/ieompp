#ifndef IEOMPP_DOT_PRODUCT_HPP_
#define IEOMPP_DOT_PRODUCT_HPP_

#include <type_traits>

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
