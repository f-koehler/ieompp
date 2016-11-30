#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/blaze_sparse_no.hpp>

template <typename Liouvillian, typename Basis, typename Lattice, typename ExpectationValue>
auto compute_matrix(const Liouvillian& L, const Basis& basis, const Lattice& lattice,
                    const ExpectationValue& ev)
{
    get_loggers().main->info("Creating {}x{} complex sparse matrix", basis.size(), basis.size());
    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    get_loggers().main->info("Computing matrix elements");
    ieompp::models::hubbard_real_space::init_matrix_no(L, M, basis, lattice, ev);
    get_loggers().main->info("  {} out of {} matrix elements are non-zero ({}% filling)",
                             M.nonZeros(), M.rows() * M.columns(),
                             double(M.nonZeros()) / (M.rows() * M.columns()));
    get_loggers().main->info("Multiply matrix with prefactor 1i");
    M *= std::complex<double>(0, 1);
    get_loggers().main->info("Finished matrix initialization");
    return M;
}

template <typename Liouvillian, typename Basis, typename Lattice>
auto compute_kinetic_matrix(const Liouvillian& L, const Basis& basis, const Lattice& lattice)
{
    get_loggers().main->info("Creating {}x{} complex sparse matrix", basis.size(), basis.size());
    blaze::CompressedMatrix<std::complex<double>, blaze::rowMajor> M(basis.size(), basis.size());
    get_loggers().main->info("Computing matrix elements");
    ieompp::models::hubbard_real_space::init_kinetic_matrix_no(L, M, basis, lattice);
    get_loggers().main->info("  {} out of {} matrix elements are non-zero ({:.5f}% filling)",
                             M.nonZeros(), M.rows() * M.columns(),
                             double(M.nonZeros()) / (M.rows() * M.columns()));
    get_loggers().main->info("Multiply matrix with prefactor 1i");
    M *= std::complex<double>(0, 1);
    get_loggers().main->info("Finished matrix initialization");
    return M;
}

#endif
