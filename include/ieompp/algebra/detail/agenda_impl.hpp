#include "ieompp/algebra/agenda.hpp"

#include <map>

namespace ieompp
{
    namespace algebra
    {
        template <typename Term>
        Agenda<Term>::Agenda(std::vector<Term>& _terms, std::list<std::size_t>& _known,
                             std::vector<std::size_t>& _todo,
                             std::vector<std::vector<Entry>> _results)
        {
            this->_terms.swap(_terms);
            this->_known.swap(_known);
            this->_todo.swap(_todo);
            this->_results.swap(_results);
        }

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
            auto pos   = _known.begin();
            bool found = false;
            for(; pos != _known.end(); ++pos) {
                if(_terms[*pos].same_operators(term)) {
                    found = true;
                    break;
                }
                if(term < _terms[*pos]) break;
            }

            return std::make_tuple(found, pos);
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

                    // get a reference to the target result entry
                    auto& result = _results[current_todo[i]];
                    auto index = *pos;

                    // find the current term in the result entry
                    auto find = std::find_if(result.begin(), result.end(),
                                             [&index](const Entry& e) { return e.index == index; });

                    if(find != result.end()) {
                        // if there is an entry for the new term, add the prefactor
                        find->prefactor += new_term.prefactor;
                    } else {
                        // create a new entry for the new term
                        result.push_back(Entry{index, new_term.prefactor});
                    }
                }
            }

            // check if further commutations are requested
            if(num > 0) commutate(num - 1, hamiltonian, discretization);
        }

        template <typename Term>
        void Agenda<Term>::join(const Agenda& agenda)
        {
            // add terms from other agenda
            std::map<std::size_t, std::size_t> index_map;
            std::size_t i = 0;
            for(auto& term : agenda._terms) {
                auto ret   = is_known(term);
                auto known = std::get<0>(ret);
                auto pos = std::get<1>(ret);
                if(known) {
                    index_map.insert(std::make_pair(i, *pos));
                    continue;
                }
                auto index = add_new_term(term, pos);
                index_map.insert(std::make_pair(i, index));
                ++i;
            }

            // copy merge todo list
            for(auto& todo : agenda._todo) {
                const auto index = index_map[todo];
                if(std::find(_todo.begin(), _todo.end(), index) == _todo.end())
                    _todo.push_back(index);
            }

            // merge results
            i = 0;
            for(auto& row : agenda._results) {
                auto& local_row = _results[index_map[i]];
                for(auto& entry : row) {
                    const auto local_index = index_map[entry.index];
                    auto pos = std::find_if(
                        local_row.begin(), local_row.end(),
                        [&local_index](const Entry& e) { return e.index == local_index; });
                    if(pos != local_row.end())
                        pos->prefactor += entry.prefactor;
                    else
                        local_row.emplace_back(Entry{local_index, entry.prefactor});
                }
                ++i;
            }
        }

        template <typename Term>
        inline const std::vector<Term>& Agenda<Term>::terms() const
        {
            return _terms;
        }

        template <typename Term>
        inline const std::list<std::size_t> Agenda<Term>::known() const
        {
            return _known;
        }

        template <typename Term>
        inline const std::vector<std::size_t> Agenda<Term>::todo() const
        {
            return _todo;
        }

        template <typename Term>
        inline const std::vector<std::vector<typename Agenda<Term>::Entry>>&
        Agenda<Term>::results() const
        {
            return _results;
        }
    }
}
