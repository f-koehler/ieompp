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
        std::tuple<bool, std::size_t> Term<Operator, Prefactor>::effect() const
        {
            const std::size_t num_created = std::count_if(
                operators.begin(), operators.end(), [](const Operator& op) { return op.creator; });
            const auto num_annihilated = operators.size() - num_created;
            const bool effect = num_created > num_annihilated;
            return std::make_tuple(effect, effect ? num_created : num_annihilated);
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

        template <typename Term, bool Erase>
        void remove_vanishing_terms(TermList<Term>& terms)
        {
            if(Erase) {
                for(std::size_t i = terms.size() - 1; i > 0; --i) {
                    if(is_zero(terms[i].prefactor)) terms.erase(terms.begin() + i);
                }
                if(is_zero(terms[0].prefactor)) terms.erase(terms.begin());
            } else {
                // TODO: fix this
                throw std::runtime_error("TODO: fix this method first");
                TermList<Term> buf(std::remove_if(terms.begin(), terms.end(), [](const Term& t) {
                                       return is_zero(t.prefactor);
                                   }), terms.end());
                std::swap(buf, terms);
            }
        }

        template <typename Term>
        void remove_forbidden(TermList<Term>& terms)
        {
            for(std::size_t i = terms.size() - 1; i > 0; --i) {
                auto& term     = terms[i];
                bool forbidden = false;
                for(auto it = term.operators.begin(); it != term.operators.end() - 1; ++it) {
                    if(*it == *(it + 1)) {
                        forbidden = true;
                        break;
                    }
                }
                if(forbidden) terms.erase(terms.begin() + i);
            }
            auto& term     = terms[0];
            bool forbidden = false;
            for(auto it = term.operators.begin(); it != term.operators.end() - 1; ++it) {
                if(*it == *(it + 1)) {
                    forbidden = true;
                    break;
                }
            }
            if(forbidden) terms.erase(terms.begin());
        }
    }
}
