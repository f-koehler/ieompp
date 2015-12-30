#ifndef HUBBARD_ABSTRACT_PARSE_HPP_
#define HUBBARD_ABSTRACT_PARSE_HPP_

#include <regex>
#include <tuple>

#include "hubbard/abstract/algebra.hpp"

namespace hubbard
{
    namespace abstract
    {
        namespace parse
        {
            static const std::regex re_commutator("^\\[\\s*(.+)\\s*;\\s*(.+)\\s*\\]$");
            static const std::regex re_operator("c_\\{([\\w\\d\\\\]+)\\,([\\w\\d\u2191\u2193\\\\]+)\\}(?:\\^\\{?(\\\\dagger|\u2020)\\}?)?");

            std::tuple<bool, std::string, std::string> is_commutator(const std::string& expression);
            std::tuple<bool, std::string, std::string, bool> is_operator(const std::string& expression);

            AbstractTerm get_operators(const std::string& expression);
        }
    }
}

#include "hubbard/abstract/parse_impl.hpp"

#endif
