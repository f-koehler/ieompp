#include <iostream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/models/hubbard_explicit/matrix_elements.hpp>
using namespace ieompp;

int main()
{
    discretization::LinearDiscretization<double, size_t> lattice(4, 1.);
    hubbard::real_space::Basis1Operator<algebra::Term<double, algebra::Operator<size_t, bool>>>
        basis(lattice);

    hubbard::real_space::MatrixElements<double> elements;

    Eigen::MatrixXd mat(basis.size(), basis.size());
    for(size_t i = 0; i < basis.size(); ++i) {
        for(size_t j = 0; j < basis.size(); ++j) {
            mat(i,j) = elements.hopping(basis[i], basis[j], lattice);
        }
    }
    cout << mat << '\n';
    return 0;
}
