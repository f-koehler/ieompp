#include "hubbard/algebra/agenda.hpp"


#include <iostream>

namespace hubbard
{
    namespace algebra
    {
        template <typename Term>
        void Agenda<Term>::reset()
        {
            // clear all lists
            _terms.clear();
            _known.clear();
            _todo.clear();
            _results.clear();
        }

        template <typename Term>
        std::tuple<bool, std::list<std::size_t>::const_iterator>
        Agenda<Term>::is_known(const Term& term) const
        {
            // find the position of the first term larger than the parameter
            auto pos = std::upper_bound(
                _known.begin(), _known.end(), term,
                [this](const Term& t, const std::size_t& i) { return t < this->_terms[i]; });

            // if the position is the beginning, there is no term smaller or equal to the parameter
            // then we have found an unknown term which will be inserted in the front
            if(pos == _known.begin()) return std::make_tuple(false, pos);

            // obtain an iterator for the previous element
            auto prev = pos;
            --prev;

            // if the previous term has the same operators as the parameter we have found a known term
            // the pos return value is of no use then
            // otherwise it is a new term and pos is the position where it should be inserted in the list
            return std::make_tuple(_terms[*prev].same_operators(term), pos);
        }

        template <typename Term>
        std::size_t Agenda<Term>::add_new_term(const Term& term, std::list<std::size_t>::const_iterator pos)
        {
            // the new term is inserted at the end of the _terms vector
            const auto new_pos = _terms.size();
            _terms.push_back(term);

            // the index is inserted at the given position in the _known list
            _known.insert(pos, new_pos);

            // a todo entry for the new term will be created
            _todo.push_back(new_pos);

            // an empty result entry is created
            _results.emplace_back(std::vector<Entry>());

            // the index of the new term is returned
            return new_pos;
        }

        template <typename Term>
        template <typename Discretization>
        void Agenda<Term>::commutate(const Term& term, const std::size_t num,
                                     const Hamiltonian<Term>& hamiltonian,
                                     const Discretization& discretization)
        {
            // reset internal lists for a fresh start
            reset();

            // add the initial term
            add_new_term(term, _known.cbegin());

            // start the recursion
            commutate(num, hamiltonian, discretization);
        }

        template <typename Term>
        template <typename Discretization>
        void Agenda<Term>::commutate(const std::size_t num, const Hamiltonian<Term>& hamiltonian,
                                     const Discretization& discretization)
        {
            // if the _todo list is empty there will be nothing left to do
            if(_todo.empty()) return;

            // swap the _todo vector with an empty vector
            // the todo entries will now be in current_todo and _todo is empty and ready to hold the
            // entries for the next recursion
            std::vector<std::size_t> current_todo;
            current_todo.swap(_todo);
            const auto size = current_todo.size();

            // iterate over all todo entries
            for(std::size_t i = 0; i < size; ++i) {

                // calculate the corresponding commutator with the hamiltonian
                const auto& term = _terms[current_todo[i]];
                const auto commutator = hamiltonian.commutate(term, discretization);

                // iterate over all terms in the commutator
                for(auto& new_term : commutator) {
                    auto ret   = is_known(new_term);
                    auto known = std::get<0>(ret);
                    auto pos   = std::get<1>(ret);

                    if(!known) {
                        // if the term is new and this is not the last commutation: add it
                        if(num > 0) {
                            auto index = add_new_term(new_term, pos);
                            _results[current_todo[i]].push_back(Entry{index, new_term.prefactor});
                        }

                        // continue with the next term in the commutator
                        continue;
                    }

                    if(*pos >= _terms.size()) {
                        std::cerr << *pos << "/" << _terms.size() << std::endl;
                        std::exit(1);
                    }

                    // if the term is known add it to the result entries for the current todo term
                    _results[current_todo[i]].push_back(Entry{*pos, new_term.prefactor});
                }
            }

            // check if further commutations are requested
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
