#ifndef HUBBARD_HPP_
#define HUBBARD_HPP_

#include <cassert>
#include <vector>

#include "ieompp/constraints.hpp"
#include "ieompp/symbolic_index.hpp"

namespace ieompp
{
    namespace momentum_space
    {
        template <typename TermT>
        struct Hamiltonian
        {
            using Term = TermT;
            using Prefactor = typename Term::Prefactor;
            using Real = typename Prefactor::value_type;

            Real J;
            Real U;
        };
    }
}

#endif
