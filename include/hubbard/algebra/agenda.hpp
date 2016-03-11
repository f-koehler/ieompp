#ifndef HUBBARD_ALGEBRA_AGENDA_HPP_
#define HUBBARD_ALGEBRA_AGENDA_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <tuple>

#include "hubbard/algebra/term.hpp"
#include "hubbard/algebra/hamiltonian.hpp"

namespace hubbard
{
    namespace algebra
    {
        template <typename TermT>
        class Agenda
        {
            using Term           = TermT;
            using Complex        = typename Term::Prefactor;
            using Real           = typename Complex::value_type;
            using TermList       = TermList<Term>;
            using Hamiltonian    = Hamiltonian<Term>;

            private:
                std::list<Term> _known_terms;
                std::vector<std::tuple<std::size_t, Term>> _todo;
                std::vector<std::tuple<Term, TermList>> _results;

            public:
                inline void reset();

                std::tuple<bool, typename std::list<Term>::iterator> is_known(const Term& term);

                std::tuple<bool, typename std::list<Term>::const_iterator> is_known(const Term& term) const;

                void add_new_term(const Term& term);

                template <typename Discretization>
                void commutate(const Term& term, const std::size_t num,
                               const Hamiltonian& hamiltonian,
                               const Discretization& discretization);

                template <typename Discretization>
                void commutate(const std::size_t num, const Hamiltonian& hamiltonian,
                               const Discretization& discretization);
        };
    }
}

#include "detail/agenda_impl.hpp"

#endif
