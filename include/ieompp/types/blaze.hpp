#ifndef IEOMPP_TYPES_BLAZE_HPP_
#define IEOMPP_TYPES_BLAZE_HPP_

#include "ieompp/string.hpp"
#include "ieompp/types/blaze/dot_product.hpp"
#include "ieompp/types/blaze/matrix_insert.hpp"
#include "ieompp/types/blaze/multiply_assign.hpp"
#include "ieompp/types/blaze/traits.hpp"

#include <blaze/system/Version.h>

namespace ieompp
{
    auto blaze_version()
    {
        static const auto str = compose(BLAZE_MAJOR_VERSION, '.', BLAZE_MINOR_VERSION);
        return str;
    }
} // namespace ieompp

#endif
