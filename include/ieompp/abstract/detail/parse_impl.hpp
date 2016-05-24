#include "ieompp/abstract/parse.hpp"

namespace ieompp
{
    namespace abstract
    {
        namespace parse_new
        {
            std::tuple<bool, std::string, std::string> parse_commutator(const std::string& expr)
            {
                std::smatch m;
                bool result = std::regex_match(expr, m, regexes::commutator);
                return std::make_tuple(result, m[1], m[2]);
            }

            AbstractTerm parse_term(std::string expr)
            {
                AbstractTerm t;
                std::smatch m_c, m_a;

                auto pos = expr.begin();
                do {
                    /* bool found_creator = std::regex_search(pos, expr.end(), m_c, regexes::creator); */
                    /* bool found_annihilator = std::regex_search(pos, expr.end(), m_c, regexes::creator); */
                } while(true);
                return t;
            }
        }

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
