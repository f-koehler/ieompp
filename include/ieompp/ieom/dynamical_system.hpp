#ifndef IEOMPP_IEOM_DYNAMICAL_SYSTEM_HPP_
#define IEOMPP_IEOM_DYNAMICAL_SYSTEM_HPP_

#include <Eigen/Dense>

#include <ieompp/exception.hpp>
#include <ieompp/ieom/basis.hpp>

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
            void create_matrix(const Term& initial, const Generator& generator)
            {
                if(basis.empty()) THROW(std::runtime_error, "basis is empty");
            }
        };
    }
}

#endif
