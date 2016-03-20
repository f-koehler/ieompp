#ifndef IEOMPP_ALGEBRA_AGENDA_HPP_
#define IEOMPP_ALGEBRA_AGENDA_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <tuple>

#include "ieompp/algebra/term.hpp"
#include "ieompp/algebra/hamiltonian.hpp"

namespace ieompp
{
    namespace algebra
    {
        template <typename TermT>
        class Agenda
        {
            using Term    = TermT;
            using Complex = typename Term::Prefactor;
            using Real    = typename Complex::value_type;

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
                void reset();

                std::tuple<bool, std::list<std::size_t>::const_iterator> is_known(const Term& term) const;
                std::size_t add_new_term(const Term& term, std::list<std::size_t>::const_iterator pos);

                template <typename Discretization>
                void commutate(const Term& term, const std::size_t num,
                               const Hamiltonian<Term>& hamiltonian,
                               const Discretization& discretization);

                template <typename Discretization>
                void commutate(const std::size_t num, const Hamiltonian<Term>& hamiltonian,
                               const Discretization& discretization);

                const std::vector<Term>& terms() const;
                const std::list<std::size_t> known() const;
                const std::vector<std::size_t> todo() const;
                const std::vector<std::vector<Entry>>& results() const;
        };
    }
}

#include "detail/agenda_impl.hpp"

#endif
