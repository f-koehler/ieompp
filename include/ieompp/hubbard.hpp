#ifndef HUBBARD_HPP_
#define HUBBARD_HPP_

#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>

#include "ieompp/constraints.hpp"
#include "ieompp/dot_product.hpp"
#include "ieompp/exception.hpp"

namespace ieompp
{
    namespace momentum_space
    {
        template <typename PrefactorT>
        struct Hamiltonian {
            using Prefactor = PrefactorT;

            Prefactor J, U;

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_kinetic_terms(const Term& t, const MomentumSpace& space,
                                        const Lattice& lattice, Container& container)
            {
                static_assert(Term::Operator::number_of_indices == 2,
                              "Hubbard model operators have two indices!");
                static_assert(
                    std::is_same<typename MomentumSpace::Vector, typename Lattice::Vector>::value,
                    "Momentum space and lattice should have the same type for their vectors!");
                if(t.operators.size() == 1)
                    generate_kinetic_terms_1(t, space, lattice, container);
                else if(t.operators.size() == 3)
                    generate_kinetic_terms_3(t, space, lattice, container);
                else
                    THROW(NotImplemented, "Currently only implemented for 1- and 3-operator terms");
            }

            template <typename Term, typename MomentumSpace, typename Lattice, typename Container>
            void generate_kinetic_terms_1(const Term& t, const MomentumSpace& space,
                                          const Lattice& lattice, Container& container)
            {
                if(t.operators.front().creator) {
                    auto momentum = space[t.operators.front().index1];

                    // TODO: check for type conversion
                    Prefactor p = 0.;
                    for(auto& lattice_vector : lattice.lattice_vectors())
                        p += std::cos(dot_product(momentum, lattice_vector));
                    p *= -2 * J;

                    Term&& new_term = t;
                    new_term.prefactpr *= p;
                    container.emplace_back(new_term);
                } else {
                    auto momentum = space[t.operators.front().index1];

                    // TODO: check for type conversion
                    Prefactor p = 0.;
                    for(auto& lattice_vector : lattice.lattice_vectors())
                        p += std::cos(dot_product(momentum, lattice_vector));
                    p *= +2 * J;

                    Term&& new_term = t;
                    new_term.prefactpr *= p;
                    container.emplace_back(new_term);
                }
            }
        };
    }
}

#endif
