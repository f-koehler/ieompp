#ifndef IEOMPP_MODELS_HUBBARD_MATRIX_ELEMENTS_HPP_
#define IEOMPP_MODELS_HUBBARD_MATRIX_ELEMENTS_HPP_

#include <cassert>
#include <tuple>

#include <ieompp/models/hubbard/basis.hpp>

namespace ieompp
{
    namespace hubbard
    {
        namespace real_space
        {
            template <typename Term, typename Lattice, typename Prefactor>
            Prefactor matrix_elements_kinetic(std::size_t i, std::size_t j,
                                              const Basis3Operator<Term>& basis,
                                              const Lattice& lattice, const Prefactor J = 1.)
            {
                if(i < basis.N) {
                    if(j < basis.N) {
                        return lattice.neighboring(i, j) ? -J : 0.;
                    } else {
                        return 0.;
                    }
                } else {
                    if(j < basis.N) {
                        return 0.;
                    } else {
                        auto& ops_i = basis[i].operators;
                        auto& ops_j = basis[j].operators;
                        if(lattice.neighboring(ops_i[0].index1, ops_j[0].index1)
                           && (ops_i[1].index1 == ops_j[1].index1)
                           && (ops_i[2].index1 == ops_j[2].index1))
                            return -J;
                        if((ops_i[0].index1 == ops_j[0].index1)
                           && lattice.neighboring(ops_i[1].index1, ops_j[1].index1)
                           && (ops_i[2].index1 == ops_j[2].index1))
                            return -J;
                        if((ops_i[0].index1 == ops_j[0].index1)
                           && (ops_i[1].index1 == ops_j[1].index1)
                           && lattice.neighboring(ops_i[2].index1, ops_j[2].index1))
                            return J;
                        return 0.;
                    }
                }
            }

            template <typename Term, typename Prefactor>
            Prefactor matrix_elements_interaction(std::size_t i, std::size_t j,
                                                  const Basis3Operator<Term>& basis,
                                                  const Prefactor U = 1.)
            {
                if(i < basis.N) {
                    if(j < basis.N) {
                        return (i == j) ? U / 2. : 0.;
                    } else {
                        return (basis.get_3op_index(i, i, i) == j) ? U / 2. : 0.;
                    }
                } else {
                    if(j < basis.N) {
                        auto& ops = basis[i].operators;
                        if((ops[0].index1 == ops[1].index1) && (ops[1].index1 == ops[2].index1)
                           && (ops[0].index1 == j))
                            return U / 2.;
                        return 0.;
                    } else {
                        return (i == j) ? U / 2. : 0.;
                    }
                }
            }
        } /* namespace real_space */
    }     /* namespace hubbard */
} /* namespace ieompp */


#endif
