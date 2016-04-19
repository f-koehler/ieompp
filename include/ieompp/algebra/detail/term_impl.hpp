#include "ieompp/algebra/term.hpp"

#include <algorithm>
#include <iterator>

namespace ieompp
{
    namespace algebra {

        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::same_operators(const Term<Operator, Prefactor>& rhs) const
        {
            if(operators.size() != rhs.operators.size())
                return false;
            return std::equal(operators.begin(), operators.end(), rhs.operators.begin());
        }

        template <typename Operator, typename Prefactor>
        inline typename std::vector<Operator>::const_iterator
        Term<Operator, Prefactor>::find_first_displaced_operator() const
        {
            for(auto it2 = operators.cbegin() + 1; it2 != operators.cend(); ++it2) {
                auto it1 = it2 - 1;
                if(*it1 > *it2) return it1;
            }
            return operators.cend();
        }

        template <typename Operator, typename Prefactor>
        inline typename std::vector<Operator>::iterator
        Term<Operator, Prefactor>::find_first_displaced_operator()
        {
            for(auto it2 = operators.begin() + 1; it2 != operators.end(); ++it2) {
                auto it1 = it2 - 1;
                if(*it1 > *it2) return it1;
            }
            return operators.end();
        }


        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::operator==(const Term<Operator, Prefactor>& rhs) const
        {
            return same_operators(rhs) && prefactor == rhs.prefactor;
        }

        template <typename Operator, typename Prefactor>
        inline bool Term<Operator, Prefactor>::operator!=(const Term<Operator, Prefactor>& rhs) const
        {
            return !same_operators(rhs) || prefactor != rhs.prefactor;
        }

        template <typename Operator, typename Prefactor>
        bool Term<Operator, Prefactor>::operator<(const Term<Operator, Prefactor>& rhs) const
        {
            if(operators.size() < rhs.operators.size()) return true;
            if(operators.size() > rhs.operators.size()) return false;
            for(auto i = operators.begin(), j = rhs.operators.begin(); i != rhs.operators.end();
                ++i, ++j) {
                if(i->creator < j->creator) return true;
                if(i->creator > j->creator) return false;
                if(i->spin < j->spin) return true;
                if(i->spin > j->spin) return false;
                if(i->index < j->index) return true;
                if(i->index > j->index) return false;
            }
            return false;
        }

        template <typename Operator, typename Prefactor>
        bool Term<Operator, Prefactor>::operator>(const Term<Operator, Prefactor>& rhs) const
        {
            if(operators.size() > rhs.operators.size()) return true;
            if(operators.size() < rhs.operators.size()) return false;
            for(auto i = operators.begin(), j = rhs.operators.begin(); i != rhs.operators.end();
                ++i, ++j) {
                if(i->creator > j->creator) return true;
                if(i->creator < j->creator) return false;
                if(i->spin > j->spin) return true;
                if(i->spin < j->spin) return false;
                if(i->index > j->index) return true;
                if(i->index < j->index) return false;
            }
            return false;
        }

        template <typename Operator, typename Prefactor>
        std::ostream& operator<<(std::ostream& strm, const Term<Operator, Prefactor>& term)
        {
            strm << term.prefactor;
            if(!term.operators.empty()) {
                strm << u8" ⋅ ";
                std::copy(term.operators.begin(), term.operators.end(),
                          std::ostream_iterator<Operator>(strm, " "));
            }
            return strm;
        }

        template <typename Term>
        void TermList<Term>::sort()
        {
            std::sort(this->begin(), this->end());
        }

        template <typename Term>
        void TermList<Term>::sum()
        {
            TermList<Term> reduced;
            for(auto& term : *this) {
                if(reduced.empty()) {
                    reduced.push_back(term);
                    continue;
                }
                if(reduced.back().same_operators(term)) {
                    reduced.back().prefactor += term.prefactor;
                    continue;
                }
                reduced.push_back(term);
            }
        }

        template <typename Prefactor, typename Operator>
        Term<Operator, Prefactor> make_term(const Prefactor& prefactor,
                                            const std::initializer_list<Operator>& operators)
        {
            return Term<Operator, Prefactor>{prefactor, operators};
        }

        template <typename Term>
        TermList<Term> order_operators(TermList<Term>& terms)
        {
            TermList<Term> results;

            std::size_t term_idx = 0;
            while(term_idx < terms.size()) {
                auto pos = terms[term_idx].find_first_displaced_operator();
                while(pos != terms[term_idx].operators.end()) {
                    auto& term = terms[term_idx];
                    /* Term new_term; */
                    /* new_term.prefactor = term.prefactor; */

                    /* std::copy(term.operators.begin(), pos, std::back_inserter(new_term.operators)); */
                    /* std::copy(pos + 2, term.operators.end(), */
                    /*           std::back_inserter(new_term.operators)); */
                    std::swap(*pos, *(pos + 1));
                    /* term.prefactor = -term.prefactor; */

                    /* terms.push_back(new_term); */

                    pos = terms[term_idx].find_first_displaced_operator();
                }

                auto& term = terms[term_idx];
                auto insert_pos =
                    std::find_if(results.begin(), results.end(),
                                 [&term](const Term& t) { return t.same_operators(term); });
                if(insert_pos == results.end())
                    results.push_back(term);
                else
                    insert_pos->prefactor += term.prefactor;
                ++term_idx;
            }

            return results;
        }
    }
}
