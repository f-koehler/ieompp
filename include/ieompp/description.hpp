#ifndef IEOMPP_DESCRIPTION_HPP_
#define IEOMPP_DESCRIPTION_HPP_

#include <ostream>
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

        Description operator+(const Description& rhs) const
        {
            Description desc(*this);
            desc += rhs;
            return desc;
        }
    };

    template <typename T>
    struct TypeDescription
    {
        static Description description()
        {
            return {{"undescribed", "type"}};
        }
    };

    template <typename T>
    struct InstanceDescription {
        static Description description(const T& t)
        {
            static_cast<void>(t);
            return {{"undescribed", "instance"}};
        }
    };

    template <typename T>
    Description get_type_description()
    {
        return TypeDescription<T>::description();
    }

    template <typename T>
    Description get_type_description(const T& t)
    {
        static_cast<void>(t);
        return TypeDescription<T>::description();
    }

    template <typename T>
    Description get_instance_description(const T& t)
    {
        return InstanceDescription<T>::description(t);
    }

    template <typename T>
    Description get_description(const T& t)
    {
        return get_type_description(t) + get_instance_description(t);
    }
}

#endif
