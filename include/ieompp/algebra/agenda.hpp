#ifndef IEOMPP_ALGEBRA_AGENDA_HPP_
#define IEOMPP_ALGEBRA_AGENDA_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <tuple>

#include "ieompp/algebra/term.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <typename HamiltonianT>
        class Agenda
        {
            public:
                using Hamiltonian = HamiltonianT;
                using Term        = typename Hamiltonian::Term;
                using Complex     = typename Term::Prefactor;
                using Real        = typename Complex::value_type;

                struct Entry {
                    std::size_t index;
                    Complex prefactor;
                };

            private:
                std::vector<Term> _terms;
                std::list<std::size_t> _known;
                std::vector<std::size_t> _todo;
                std::vector<std::vector<Entry>> _results;

            public:
                Agenda() = default;
                Agenda(std::vector<Term>& _terms, std::list<std::size_t>& _known,
                       std::vector<std::size_t>& _todo, std::vector<std::vector<Entry>> _results);

                void reset();

                std::tuple<bool, std::list<std::size_t>::const_iterator> is_known(const Term& term) const;
                std::size_t add_new_term(const Term& term, std::list<std::size_t>::const_iterator pos);

                template <typename Discretization>
                void commutate(const Term& term, const std::size_t num,
                               const Hamiltonian& hamiltonian,
                               const Discretization& discretization);

                template <typename Discretization>
                void commutate(const std::size_t num, const Hamiltonian& hamiltonian,
                               const Discretization& discretization);

                void join(const Agenda& agenda);

                inline const std::vector<Term>& terms() const;
                inline const std::list<std::size_t> known() const;
                inline const std::vector<std::size_t> todo() const;
                inline const std::vector<std::vector<Entry>>& results() const;
        };

        template <typename Term>
        std::ostream& operator<<(std::ostream& strm, const Agenda<Term>& agenda);
    }
}

#include "detail/agenda_impl.hpp"

#endif
