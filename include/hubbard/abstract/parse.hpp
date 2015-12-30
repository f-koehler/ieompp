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
                static const std::string greek           = u8"α-ω";
                static const std::string greek_capital   = u8"\u0391-\u03a9";
                static const std::string arrows          = u8"←-↓";

                static const std::string index =
                    letters + letters_capital + numbers + greek + greek_capital + "_'\\\\";
                static const std::string spin =
                    letters + letters_capital + numbers + greek + greek_capital + "_'\\\\" + arrows;

                static const std::string str_commutator = "^\\[\\s*(.+)\\s*;\\s*(.+)\\s*\\]$";
                static const std::string str_operator =
                    "c_\\{\\s*([" + index + "]+)\\s*\\,\\s*([" + spin
                    + u8"]+)\\s*\\}(?:\\^\\{?(\\\\dagger|†)\\}?)?";
                static const std::string str_kronecker =
                    u8"(?:δ|\\\\delta)_\\{\\s*([" + spin + "]+)\\s*\\,\\s*([" + spin + "]+)\\s*\\}";
                static const std::string str_number = "\\(\\s*([\\deE\\.\\+-]+)\\s*\\,\\s*([\\deE\\.\\+-]+)\\s*\\)";
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
