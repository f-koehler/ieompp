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
            namespace components {
                static const std::string letters         = u8"a-z";
                static const std::string letters_capital = u8"A-Z";
                static const std::string numbers         = u8"0-9";
                static const std::string greek           = u8"αβγδεζηθικλμνξοπρστφχψω";
                static const std::string greek_capital   = u8"\u0391-\u03a9";
                static const std::string arrows          = u8"←↑↓→";

                static const std::string index = letters + letters_capital + numbers + greek + greek_capital + "_'\\\\\\+\\-";
                static const std::string spin  = letters + letters_capital + numbers + greek + greek_capital + arrows + "_'\\\\\\+\\-";

                static const std::string commutator_term     = u8"\\s*(.+)\\s*";
                static const std::string operator_exponent   = u8"(?:\\^\\{?(\\\\dagger|†)\\}?)?";
                static const std::string operator_subscript  = u8"_\\{\\s*([" + index + u8"]+)\\s*\\,\\s*([" + spin + u8"]+)\\s*\\}";
                static const std::string kronecker_subscript = u8"_\\{\\s*([" + spin + u8"]+)\\s*\\,\\s*([" + spin + u8"]+)\\s*\\}";

                static const std::string str_commutator = u8"^\\[" + commutator_term + ";" + commutator_term + "\\]$";
                static const std::string str_operator   = u8"c" + operator_subscript + operator_exponent;
                static const std::string str_kronecker  = u8"(?:δ|\\\\delta)" + kronecker_subscript;
                static const std::string str_number     = u8"\\(\\s*([\\deE\\.\\+-]+)\\s*\\,\\s*([\\deE\\.\\+-]+)\\s*\\)";
            }

            static const std::regex re_commutator(components::str_commutator);
            static const std::regex re_operator(components::str_operator);
            static const std::regex re_kronecker(components::str_kronecker);
            static const std::regex re_number(components::str_number);

            std::tuple<bool, std::string, std::string> is_commutator(const std::string& expression);
            std::tuple<bool, std::string, std::string, bool> is_operator(const std::string& expression);
            std::tuple<bool, std::string, std::string> is_kronecker(const std::string& expression);
            std::tuple<bool, std::string, std::string> is_number(const std::string& expression);

            AbstractTerm parse_term(const std::string& expression);
        }
    }
}

#include "hubbard/abstract/parse_impl.hpp"

#endif
