#include "ieompp/ieom/agenda.hpp"
#include "ieompp/algebra/commutator.hpp"

#include <map>

namespace ieompp
{
    namespace ieom
    {
        template <typename Hamiltonian>
        void Agenda<Hamiltonian>::reset()
        {
            // clear all lists
            _sys.terms.clear();
            _sys.coefficients.clear();
            _known.clear();
            _todo.clear();
        }

        template <typename Hamiltonian>
        std::tuple<bool, std::list<std::size_t>::const_iterator>
        Agenda<Hamiltonian>::is_known(const Term& term) const
        {
            auto pos   = _known.begin();
            bool found = false;
            for(; pos != _known.end(); ++pos) {
                if(_sys.terms[*pos].same_operators(term)) {
                    found = true;
                    break;
                }
                if(term < _sys.terms[*pos]) break;
            }

            return std::make_tuple(found, pos);
        }

        template <typename Hamiltonian>
        std::size_t Agenda<Hamiltonian>::add_new_term(const Term& term,
                                                      std::list<std::size_t>::const_iterator pos)
        {
            // the new term is inserted at the end of the _terms vector
            const auto new_pos = _sys.terms.size();
            _sys.terms.push_back(term);

            // the index is inserted at the given position in the _known list
            _known.insert(pos, new_pos);

            // a todo entry for the new term will be created
            _todo.push_back(new_pos);

            // an empty result entry is created
            _sys.coefficients.emplace_back(std::vector<Coefficient>());

            // the index of the new term is returned
            return new_pos;
        }

        template <typename Hamiltonian>
        template <typename Discretization>
        void Agenda<Hamiltonian>::commutate(const Term& term, const std::size_t num,
                                            const Hamiltonian& hamiltonian,
                                            const Discretization& discretization)
        {
            // reset internal lists for a fresh start
            reset();

            // add the initial term
            add_new_term(term, _known.cbegin());

            // start the recursion
            commutate(num, hamiltonian, discretization);
        }

        template <typename Hamiltonian>
        template <typename Discretization>
        void Agenda<Hamiltonian>::commutate(const std::size_t num, const Hamiltonian& hamiltonian,
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
                const auto& term    = _sys.terms[current_todo[i]];
                auto commutator_tmp = hamiltonian.commutate(term, discretization);
                auto commutator = algebra::order_operators(commutator_tmp);
                commutator_tmp.clear();
                algebra::remove_vanishing_terms(commutator);

                // iterate over all terms in the commutator
                for(auto& new_term : commutator) {
                    auto ret   = is_known(new_term);
                    auto known = std::get<0>(ret);
                    auto pos   = std::get<1>(ret);

                    if(!known) {
                        // if the term is new and this is not the last commutation: add it
                        if(num > 0) {
                            auto index = add_new_term(new_term, pos);
                            _sys.coefficients[current_todo[i]].push_back(
                                Coefficient{index, new_term.prefactor});
                        }

                        // continue with the next term in the commutator
                        continue;
                    }

                    // get a reference to the target result entry
                    auto& result = _sys.coefficients[current_todo[i]];
                    auto index   = *pos;

                    // find the current term in the result entry
                    auto find =
                        std::find_if(result.begin(), result.end(),
                                     [&index](const Coefficient& e) { return e.index == index; });

                    if(find != result.end()) {
                        // if there is an entry for the new term, add the prefactor
                        find->prefactor += new_term.prefactor;
                    } else {
                        // create a new entry for the new term
                        result.push_back(Coefficient{index, new_term.prefactor});
                    }
                }
            }

            // check if further commutations are requested
            if(num > 0) commutate(num - 1, hamiltonian, discretization);
        }

        template <typename Hamiltonian>
        void Agenda<Hamiltonian>::join(const Agenda& agenda)
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
            for(auto& row : agenda._sys.coefficients) {
                auto& local_row = _sys.coefficients[index_map[i]];
                for(auto& entry : row) {
                    const auto local_index = index_map[entry.index];
                    auto pos = std::find_if(
                        local_row.begin(), local_row.end(),
                        [&local_index](const Coefficient& e) { return e.index == local_index; });
                    if(pos != local_row.end())
                        pos->prefactor += entry.prefactor;
                    else
                        local_row.emplace_back(Coefficient{local_index, entry.prefactor});
                }
                ++i;
            }
        }

        template <typename Hamiltonian>
        inline const typename Agenda<Hamiltonian>::System&
        Agenda<Hamiltonian>::system() const
        {
            return _sys;
        }

        template <typename Hamiltonian>
        inline const std::list<std::size_t> Agenda<Hamiltonian>::known() const
        {
            return _known;
        }

        template <typename Hamiltonian>
        inline const std::vector<std::size_t> Agenda<Hamiltonian>::todo() const
        {
            return _todo;
        }

        template <typename Hamiltonian>
        std::ostream& operator<<(std::ostream& strm, const Agenda<Hamiltonian>& agenda)
        {
            strm << "terms:" << std::endl;
            for(auto& term : agenda.system().terms) strm << term << std::endl;
            strm << std::endl << std::endl;

            strm << "results:" << std::endl;
            std::size_t i = 0;
            for(auto& line : agenda.system().coefficients) {
                strm << agenda.system().terms[i] << ":" << std::endl;
                for(auto& entry : line) {
                    auto term      = agenda.system().terms[entry.index];
                    term.prefactor = entry.prefactor;
                    strm << "\t" << term << std::endl;
                }
                ++i;
                strm << std::endl;
            }
            return strm;
        }
    }
}
