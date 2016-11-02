#ifndef IEOMPP_TYPES_EIGEN_HPP_
#define IEOMPP_TYPES_EIGEN_HPP_

#include "ieompp/string.hpp"
#include "ieompp/types/eigen/description.hpp"
#include "ieompp/types/eigen/dot_product.hpp"
#include "ieompp/types/eigen/init.hpp"
#include "ieompp/types/eigen/traits.hpp"

#include <Eigen/src/Core/util/Macros.h>

namespace ieompp
{
    auto eigen_version()
    {
        static const auto str =
            compose(EIGEN_WORLD_VERSION, '.', EIGEN_MAJOR_VERSION, '.', EIGEN_MINOR_VERSION);
        return str;
    }
}

#endif
