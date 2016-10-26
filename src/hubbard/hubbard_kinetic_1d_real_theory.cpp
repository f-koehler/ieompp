#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
using namespace std::complex_literals;

#include <boost/math/special_functions/bessel.hpp>

#include <omp.h>

#include <ieompp/constants.hpp>
#include <ieompp/discretization/linear.hpp>

int main()
{
    const uint64_t N = 64;
    ieompp::discretization::LinearDiscretization<double> lattice(N, 1.);

    const double dt      = 0.01;
    const uint64_t steps = 100000;

    vector<vector<complex<double>>> h_vals(N), h_vals_conj(N);

#pragma omp parallel for
    for(uint64_t i = 0; i < N; ++i) {
        h_vals[i].resize(steps);
        h_vals_conj[i].resize(steps);
        std::complex<double> prefactor;
        switch(i % 4) {
            case 0:
                prefactor = 1.;
                break;
            case 1:
                prefactor = -1.i;
                break;
            case 2:
                prefactor = -1.;
                break;
            default:
                prefactor = 1.i;
        }
        for(uint64_t step = 0; step < steps; ++step) {
            double bess          = boost::math::cyl_bessel_j(i, 2 * step * dt);
            h_vals[i][step]      = bess * prefactor;
            h_vals_conj[i][step] = bess * std::conj(prefactor);
        }
    }

    vector<vector<complex<double>>> results;
#pragma omp parallel
    {
#pragma omp critical
        {
            results.emplace_back(steps, complex<double>(0.));
        }
    }

    static const auto pi = ieompp::Pi<double>::value;

#pragma omp parallel for
    for(uint64_t i = 0; i < N; ++i) {
        for(uint64_t j = 0; j < N; ++j) {
            double ev = 0.;
            if(i == j) {
                ev = 0.5;
            } else {
                const auto dist = lattice.lattice_distance(i, j);
                if(dist % 2 == 0) {
                    continue;
                }
                if(((dist - 1) / 2) % 2 == 0) {
                    ev = 1 / (dist * pi);
                }
                ev = -1 / (dist * pi);
            }

            for(uint64_t step = 0; step < steps; ++step) {
                results[omp_get_thread_num()][step] += ev * (h_vals[i][step] * h_vals_conj[j][step]);
            }
        }
    }

    ofstream file("theory.txt");
    for(uint64_t step = 0; step < steps; ++step) {
        complex<double> val = 0.;
        for(auto & result : results) { val += result[step];
}
        file << step * dt << '\t' << val.real() << '\t' << val.imag() << '\n';
    }
    file.close();
}
