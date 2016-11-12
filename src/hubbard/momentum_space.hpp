#ifndef SRC_HUBBARD_MOMENTUM_SPACE_HPP_
#define SRC_HUBBARD_MOMENTUM_SPACE_HPP_

#include "application.hpp"
#include "common.hpp"

#include <ieompp/models/hubbard_momentum_space/blaze_sparse.hpp>

template <typename Liouvillian, typename Basis, typename Lattice>
blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor>
compute_matrix(const Liouvillian& L, const Basis& basis, const Lattice& lattice)
{
    get_loggers().main->info("Creating {}x{} complex sparse matrix", basis.size(), basis.size());
    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    get_loggers().main->info("Computing matrix elements");
    ieompp::models::hubbard_momentum_space::init_matrix(L, M, basis, lattice);
    get_loggers().main->info("  {} out of {} matrix elements are non-zero ({:.5f}% filling)",
                             M.nonZeros(), M.rows() * M.columns(),
                             double(M.nonZeros()) / (M.rows() * M.columns()));
    get_loggers().main->info("Multiply matrix with prefactor 1i");
    M *= std::complex<double>(0, 1);
    get_loggers().main->info("Finished matrix initialization");
    return M;
}

#endif
