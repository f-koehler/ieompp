#ifndef HUBBARD_ABSTRACT_PARSE_HPP_
#define HUBBARD_ABSTRACT_PARSE_HPP_

#include <tuple>

#include "hubbard/abstract/algebra.hpp"

namespace hubbard
{
    namespace abstract
    {
        namespace parse
        {
            std::tuple<bool, std::string, std::string> is_commutator(const std::string& expression);
            AbstractTerm get_operators(const std::string& expression);
        }
    }
}

#include "hubbard/abstract/parse_impl.hpp"

#endif
