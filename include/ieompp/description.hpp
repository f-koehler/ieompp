#ifndef IEOMPP_DESCRIPTION_HPP_
#define IEOMPP_DESCRIPTION_HPP_

#include <string>
#include <utility>
#include <vector>

namespace ieompp
{
    using DescriptionEntry = std::pair<std::string, std::string>;
    using Description      = std::vector<DescriptionEntry>;

    template <typename T>
    struct VariableDescription {
        static Description get(const T& t)
        {
            static_cast<void>(t);
            return {};
        }
    };

    template <typename T>
    Description get_description(const T& t)
    {
        return VariableDescription<T>::get(t);
    }
}

#endif
