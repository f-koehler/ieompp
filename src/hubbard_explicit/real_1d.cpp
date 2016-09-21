#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_explicit/matrix_elements.hpp>
#include <ieompp/types/number.hpp>
using namespace ieompp::algebra;

#include <Eigen/Sparse>

int main()
{
    const size_t N = 30;

    ieompp::discretization::LinearDiscretization<double, size_t> lattice(N, 1.);

    using Operator = Operator<size_t, bool>;
    using Term     = Term<double, Operator>;

    // init operator basis
    vector<Term> basis;
    for(auto r : lattice) {
        basis.push_back(make_term(1., {make_creator(r, true)}));
    }
    for(auto r1 : lattice) {
        for(auto r2 : lattice) {
            for(auto r3 : lattice) {
                basis.push_back(make_term(1., {make_creator(r1, true), make_creator(r2, false),
                                               make_annihilator(r3, false)}));
            }
        }
    }
    const auto basis_size = basis.size();

    ieompp::hubbard::real_space::MatrixElements<double> elements{1., 1.};
    Eigen::SparseMatrix<double> matrix(basis_size, basis_size);

    for(auto i = 0ul; i < basis_size; ++i) {
        for(auto j = 0ul; j < basis_size; ++j) {
            const auto val = elements.interaction(basis[i], basis[j])
                             + elements.hopping(basis[i], basis[j], lattice);
            if(!ieompp::types::is_zero(val)) matrix.insert(i, j) = val;
        }
    }
}
