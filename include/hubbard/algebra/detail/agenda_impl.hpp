#include "hubbard/algebra/agenda.hpp"


namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        void Agenda<Term>::reset()
        {
            _terms.clear();
            _known.clear();
            _todo.clear();
            _results.clear();
        }

        template <typename Term>
        std::tuple<bool, std::list<std::size_t>::const_iterator>
        Agenda<Term>::is_known(const Term& term) const
        {
            auto pos = std::upper_bound(
                _known.begin(), _known.end(), term,
                [this](const Term& t, const std::size_t& i) { return t < this->_terms[i]; });
            if(pos == _known.begin()) return std::make_tuple(false, pos);
            auto prev = pos;
            --prev;
            return std::make_tuple(_terms[*prev].same_operators(term), pos);
        }

        template <typename Term>
        std::size_t Agenda<Term>::add_new_term(const Term& term, std::list<std::size_t>::const_iterator pos)
        {
            auto new_pos = _terms.size();
            _terms.push_back(term);
            _known.insert(pos, new_pos);
            _todo.push_back(new_pos);
            _results.emplace_back(std::vector<Entry>());
            return new_pos;
        }

        template <typename Term>
        template <typename Discretization>
        void Agenda<Term>::commutate(const Term& term, const std::size_t num,
                                     const Hamiltonian<Term>& hamiltonian,
                                     const Discretization& discretization)
        {
            reset();
            add_new_term(term, _known.cbegin());
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
                const auto& term = _terms[_todo[i]];
                const auto commutator = hamiltonian.commutate(term, discretization);

                for(auto& new_term : commutator) {
                    auto ret   = is_known(new_term);
                    auto known = std::get<0>(ret);
                    auto pos   = std::get<1>(ret);

                    if(!known) {
                        if(num > 0) {
                            auto index = add_new_term(new_term, pos);
                            _results[i].push_back(Entry{index, new_term.prefactor});
                        }
                        continue;
                    }
                    _results[i].push_back(Entry{*pos, new_term.prefactor});
                }
            }
            _todo.erase(_todo.begin(), _todo.begin() + size);

            if(num > 0) commutate(num - 1, hamiltonian, discretization);
        }

        template <typename Term>
        const std::vector<Term>& Agenda<Term>::terms() const
        {
            return _terms;
        }

        template <typename Term>
        const std::vector<std::vector<typename Agenda<Term>::Entry>>& Agenda<Term>::results() const
        {
            return _results;
        }
    }
}
