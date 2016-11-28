#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
using namespace std::complex_literals;

#include <boost/math/special_functions/bessel.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <ieompp/algebra/monomial.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/constants.hpp>
#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_real_space.hpp>
#include <ieompp/openmp.hpp>
namespace hubbard = ieompp::models::hubbard_real_space;

complex<double> minus_i_power(uint64_t power)
{
    switch(power % 4) {
        case 0:
            return 1.;
        case 1:
            return -1.i;
        case 2:
            return -1.;
        default:
            return 1.i;
    }
}

int main()
{
    const uint64_t N = 128;
    ieompp::lattices::PeriodicChain<double> lattice(N, 1.);
    hubbard::Basis1Operator<ieompp::algebra::Monomial<ieompp::algebra::Operator<uint64_t, bool>>>
        basis(lattice);

    const double dt      = 0.01;
    const uint64_t steps = 10000;

    // precompute time dependent prefactors and their complex conjugates
    vector<vector<complex<double>>> h_vals(N), h_vals_conj(N);
#pragma omp parallel for
    for(uint64_t i = 0; i < N; ++i) {
        h_vals[i].resize(steps);
        h_vals_conj[i].resize(steps);
        const auto j                   = lattice.lattice_distance(0, i);
        std::complex<double> prefactor = minus_i_power(j);
        for(uint64_t step = 0; step < steps; ++step) {
            double bess          = boost::math::cyl_bessel_j(j, 2 * step * dt);
            h_vals[i][step]      = bess * prefactor;
            h_vals_conj[i][step] = bess * std::conj(prefactor);
        }
    }

    const hubbard::ExpectationValue1DHalfFilled<double, decltype(lattice)> expectation_value(
        lattice);

    vector<vector<complex<double>>> results;
#pragma omp parallel
    {
#pragma omp critical
        {
            results.emplace_back(steps, complex<double>(0.));
        }
    }

#pragma omp parallel for
    for(uint64_t i = 0; i < N; ++i) {
        for(uint64_t j = 0; j < N; ++j) {
            const auto ev = expectation_value(basis[i].front(), basis[j].front());
            for(uint64_t step = 0; step < steps; ++step) {
                results[omp_get_thread_num()][step] +=
                    ev * (h_vals[i][step] * h_vals_conj[j][step]);
            }
        }
    }

    ofstream file("theory.txt", ofstream::trunc);
    for(uint64_t step = 0; step < steps; ++step) {
        complex<double> val = 0.;
        for(auto &result : results) {
            val += result[step];
        }
        file << step * dt << '\t' << val.real() << '\t' << val.imag() << '\n';
    }
    file.close();
}
