#ifndef IEOMPP_DESCRIPTION_HPP_
#define IEOMPP_DESCRIPTION_HPP_

#include <string>
#include <utility>
#include <vector>

#include <ieompp/string.hpp>

namespace ieompp
{
    using DescriptionEntry = std::pair<std::string, std::string>;

    struct Description : public std::vector<DescriptionEntry> {
        using std::vector<DescriptionEntry>::vector;

        Description& indent(std::size_t n)
        {
            for(auto& entry : *this) {
                entry.first.insert(0, n, ' ');
            }
            return *this;
        }

        Description& operator+=(const Description& rhs)
        {
            std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this));
            return *this;
        }
    };

    template <typename T>
    struct VariableDescription {
        static Description get(const T& t)
        {
            static_cast<void>(t);
            return {};
        }
    };

    template <typename T>
    struct TypeProperties {
    };

    template <typename T>
    struct TypeDescription {
        static Description get()
        {
            return {{"type:", compose(TypeProperties<T>::name, ' ', "(size ",
                                      TypeProperties<T>::size, ")")}};
        }
    };

    template <typename T>
    Description get_description(const T& t)
    {
        return VariableDescription<T>::get(t);
    }

    template <typename T>
    Description get_description()
    {
        return TypeDescription<T>::get();
    }
}

#endif
