#include "hubbard/abstract/parse.hpp"

namespace hubbard
{
    namespace abstract
    {
        namespace parse
        {
            std::tuple<bool, std::string, std::string> is_commutator(const std::string& expression)
            {
                std::smatch m;
                if(!std::regex_match(expression, m, re_commutator))
                    std::make_tuple(false, "", "");

                return std::make_tuple(true, m[1].str(), m[2].str());
            }

            std::tuple<bool, std::string, std::string, bool> is_operator(const std::string& expression)
            {
                std::smatch m;
                if(!std::regex_match(expression, m, re_operator))
                    return std::make_tuple(false, "", "", false);
                bool creator = !m[3].str().empty();
                return std::make_tuple(true, m[1].str(), m[2].str(), creator);
            }

            AbstractTerm get_operators(const std::string& expression)
            {
                std::vector<std::string> tokens;
                std::istringstream iss(expression);
                std::copy(std::istream_iterator<std::string>(iss),
                          std::istream_iterator<std::string>(), std::back_inserter(tokens));

                AbstractTerm term;
                term.prefactor.prefactor = Complex(1., 0.);

                std::smatch m;
                for(auto& tok : tokens) {
                    if(!std::regex_match(tok, m, re_operator))
                        continue; // TODO: exception
                    auto index = m[1].str();
                    auto spin = m[2].str();
                    if(!m[3].str().empty())
                        term.operators.push_back(make_creator(index, spin));
                    else
                        term.operators.push_back(make_annihilator(index, spin));
                }

                return term;
            }
        }
    }
}
