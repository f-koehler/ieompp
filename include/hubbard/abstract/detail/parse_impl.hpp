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
            
            std::tuple<bool, std::string, std::string> is_kronecker(const std::string& expression)
            {
                std::smatch m;
                if(!std::regex_match(expression, m, re_kronecker))
                    return std::make_tuple(false, "", "");
                return std::make_tuple(true, m[1].str(), m[2].str());
            }
            
            std::tuple<bool, std::string, std::string> is_number(const std::string& expression)
            {
                std::smatch m;
                if(!std::regex_match(expression, m, re_number))
                    return std::make_tuple(false, "", "");
                return std::make_tuple(true, m[1].str(), m[2].str());
            }

            AbstractTerm parse_term(const std::string& expression)
            {
                AbstractTerm term;
                term.prefactor.number = std::complex<double>(1., 0.);

                std::string tmp = expression;
                std::smatch m;
                while(std::regex_search(tmp, m, re_operator)) {
                    if(m[3].str().empty())
                        term.operators.push_back(make_annihilator(m[1].str(), m[2].str()));
                    else
                        term.operators.push_back(make_creator(m[1].str(), m[2].str()));
                    tmp = m.suffix();
                }

                tmp = expression;
                while(std::regex_search(tmp, m, re_kronecker)) {
                    term.prefactor.kroneckers.push_back(Kronecker{m[1].str(), m[2].str()});
                    tmp = m.suffix();
                }

                tmp = expression;
                while(std::regex_match(tmp, m, re_number)) {
                    term.prefactor.number *=
                        std::complex<double>(std::stod(m[1].str()), std::stod(m[2].str()));
                }

                return term;
            }
        }
    }
}
