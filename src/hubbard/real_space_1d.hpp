#ifndef SRC_HUBBARD_REAL_SPACE_1d_HPP_
#define SRC_HUBBARD_REAL_SPACE_1d_HPP_

#include "real_space.hpp"

#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_real_space/basis.hpp>

using Basis1  = ieompp::models::hubbard_real_space::Basis1Operator<Term1d>;
using Basis3  = ieompp::models::hubbard_real_space::Basis3Operator<Term1d>;
using Lattice = ieompp::discretization::LinearDiscretization<double, uint64_t>;

#endif
