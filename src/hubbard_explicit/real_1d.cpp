#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_explicit/matrix_elements.hpp>
#include <ieompp/types/eigen_init.hpp>
using namespace ieompp::algebra;

#include <Eigen/Sparse>

int main()
{
    Eigen::initParallel();

    const size_t N = 4;

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

    auto generator = [&elements, &basis, &lattice](std::size_t i, std::size_t j) {
        return elements.hopping(basis[i], basis[j], lattice)
               + elements.interaction(basis[i], basis[j]);
    };
    auto matrix = ieompp::types::init_parallel<Eigen::SparseMatrix<double>>(basis_size, basis_size,
                                                                            generator);
    cout << matrix << '\n';
}
