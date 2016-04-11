#ifndef IEOMPP_IEOM_AGENDA_HPP_
#define IEOMPP_IEOM_AGENDA_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <ostream>
#include <tuple>

#include "ieompp/ieom/system.hpp"

namespace ieompp
{
    namespace ieom
    {
        template <typename HamiltonianT>
        class Agenda
        {
            public:
                using Hamiltonian = HamiltonianT;
                using Term        = typename Hamiltonian::Term;
                using Complex     = typename Term::Prefactor;
                using Real        = typename Complex::value_type;
                using System      = DynamicalSystem<Term, Complex>;
                using Coefficient = typename System::Coefficient;

            private:
                std::list<std::size_t> _known;
                std::vector<std::size_t> _todo;
                System _sys;

            public:
                Agenda() = default;

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

                const System& system() const;
                const std::list<std::size_t> known() const;
                const std::vector<std::size_t> todo() const;
        };

        template <typename Term>
        std::ostream& operator<<(std::ostream& strm, const Agenda<Term>& agenda);
    }
}

#include "detail/agenda_impl.hpp"

#endif
