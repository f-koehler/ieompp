#ifndef IEOMPP_IEOM_DYNAMICAL_SYSTEM_HPP_
#define IEOMPP_IEOM_DYNAMICAL_SYSTEM_HPP_

#include "ieompp/exception.hpp"
#include "ieompp/ieom/basis.hpp"

#include <Eigen/Dense>

namespace ieompp
{
    namespace ieom
    {
        template <typename TermT>
        struct DynamicalSystem {
            using Term      = TermT;
            using Prefactor = typename Term::Prefactor;
            using Matrix    = Eigen::Matrix<Prefactor, Eigen::Dynamic, Eigen::Dynamic>;

            Basis<Term> basis;
            Matrix matrix;

            template <typename Generator>
            void create_matrix(const Generator& generator)
            {
                if(basis.empty()) THROW(std::runtime_error, "basis is empty");

                // we will only work on a copy of the basis
                const Basis<Term>& basis = this->basis;
                const auto size          = basis.size();

                matrix = Matrix::Zero(size, size);

                for(std::size_t i = 0; i < size; ++i) {
                    std::vector<Term> generated;
                    generator(basis[i], generated);
                    for(const auto& t : generated) {
                        auto basis_pos = basis.find(t);
                        if(basis_pos == basis.end()) continue;
                        matrix(i, basis_pos - basis.begin()) = t.prefactor;
                    }
                }
                matrix *= std::complex<double>(0., 1.);
            }
        };
    }
}

#endif
