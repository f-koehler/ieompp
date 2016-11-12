#ifndef SRC_HUBBARD_MOMENTUM_SPACE_1D_HPP_
#define SRC_HUBBARD_MOMENTUM_SPACE_1D_HPP_

#include "momentum_space.hpp"

#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_momentum_space/basis.hpp>

using Basis3        = ieompp::models::hubbard_momentum_space::Basis3Operator<Term1d>;
using MomentumSpace = ieompp::discretization::LinearDiscretization<double, uint64_t>;
using Lattice       = ieompp::discretization::LinearDiscretization<double, uint64_t>;

#endif
