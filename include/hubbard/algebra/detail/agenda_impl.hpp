#include "hubbard/algebra/agenda.hpp"


namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        inline void Agenda<Term>::reset()
        {
            _known_terms.clear();
            _todo.clear();
            _results.clear();
        }

        template <typename Term>
        std::tuple<bool, typename std::list<Term>::iterator>
        Agenda<Term>::is_known(const Term& term)
        {
            const auto pos = std::upper_bound(_known_terms.begin(), _known_terms.end(), term);
            if(pos == _known_terms.begin()) return std::make_tuple(false, pos);
            auto prev = pos;
            --prev;
            return std::make_tuple(prev->same_operators(term), pos);
        }

        template <typename Term>
        std::tuple<bool, typename std::list<Term>::const_iterator>
        Agenda<Term>::is_known(const Term& term) const
        {
            const auto pos = std::upper_bound(_known_terms.cbegin(), _known_terms.cend(), term);
            if(pos == _known_terms.begin()) return std::make_tuple(false, pos);
            return std::make_tuple((pos - 1)->same_operators(term), pos);
        }

        template <typename Term>
        std::size_t Agenda<Term>::add_new_term(const Term& term)
        {
            _known_terms.push_back(term);
            auto size = _results.size();
            _todo.push_back(std::make_tuple(size, term));
            _results.push_back(std::make_tuple(term, TermList<Term>()));
            return size;
        }

        template <typename Term>
        template <typename Discretization>
        void Agenda<Term>::commutate(const Term& term, const std::size_t num,
                                     const Hamiltonian<Term>& hamiltonian,
                                     const Discretization& discretization)
        {
            add_new_term(term);
            commutate(num, hamiltonian, discretization);
        }

        template <typename Term>
        template <typename Discretization>
        void Agenda<Term>::commutate(const std::size_t num, const Hamiltonian<Term>& hamiltonian,
                                     const Discretization& discretization)
        {
            if(_todo.empty()) return;

            const auto size = _todo.size();
            for(std::size_t i = 0; i < size; ++i) {
                auto& todo = _todo[i];
                auto index = std::get<0>(todo);
                auto& term = std::get<1>(todo);

                const auto comm = hamiltonian.commutate(term, discretization);
                for(auto& new_term : comm) {
                    const auto known = is_known(new_term);
                    if(std::get<0>(known)) {
                        std::get<1>(_results[index]).push_back(new_term);
                        continue;
                    }
                    if(num == 0) continue;
                    std::get<1>(_results[index]).push_back(new_term);
                    add_new_term(new_term);
                }
            }

            _todo.erase(_todo.begin(), _todo.begin() + size);

            if(num != 0)
                commutate(num - 1, hamiltonian, discretization);
            else {
                for(auto& entry : _results) {
                    auto& list = std::get<1>(entry);
                    list.sort();
                    list.sum();
                }
            }
        }

        template <typename Term>
        inline auto Agenda<Term>::results() const -> const decltype(_results) &
        {
            return _results;
        }
    }
}
