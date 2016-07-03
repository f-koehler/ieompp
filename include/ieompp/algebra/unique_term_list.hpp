#ifndef IEOMPP_ALGEBRA_UNIQUE_TERM_LIST_HPP_
#define IEOMPP_ALGEBRA_UNIQUE_TERM_LIST_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <type_traits>

#include <ieompp/algebra/term_comparison.hpp>

namespace ieompp
{
    namespace algebra
    {
        template <typename Term>
        class UniqueTermList : public std::list<Term>
        {
            private:
                const TermSmaller<Term> _term_comp;

            public:
                void push_back(const Term& value)  = delete;
                void push_back(Term&& value)       = delete;
                void push_front(const Term& value) = delete;
                void push_front(Term&& value)      = delete;

                template <typename... Args>
                void emplace_back(Args&&... args)  = delete;
                template <typename... Args>
                void emplace_front(Args&&... args) = delete;

                typename std::list<Term>::iterator add(const Term& term)
                {
                    auto pos =
                        std::binary_search(this->begin(), this->end(), std::bind(_term_comp, term));
                    if(pos->same_operators(term)) pos->prefactor += term.prefactor;
                    this->insert(pos, term);
                    return pos;
                }
        };
    }
}

#endif
