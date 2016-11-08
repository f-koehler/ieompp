#include <complex>
#include <fstream>
#include <iostream>
using namespace std;

#include <ieompp/ode/rk4.hpp>
#include <ieompp/types/blaze.hpp>
#include <ieompp/constants.hpp>

complex<double> particle_number(const blaze::DynamicVector<complex<double>>& h)
{
    static const double pi_inv = 1. / ieompp::Pi<double>::value;

    complex<double> ev(0., 0.);
    for(uint64_t i = 0; i < 4; ++i) {
        for(uint64_t j = 0; j < 4; ++j) {
            if(i == j) {
                if(i < 3) {
                    ev += norm(h[i]) * 0.5;
                } else {
                    ev += norm(h[i]) * 0.25;
                }
                continue;
            }

            if((i < 3) && (j < 3)) {
                ev += (h[i] * h[j]) * pi_inv;
                continue;
            }

            if((i == 0) || (j == 0)) {
                ev += (h[i] * h[j]) * 0.25;
            } else {
                ev += (h[i] * h[j]) * 0.5 * pi_inv;
            }
        }
    }
    return ev;
}

void write_state(ostream& strm, const double& t, const blaze::DynamicVector<complex<double>>& h)
{
    complex<double> n = particle_number(h);
    strm << t;
    strm << '\t' << h[0].real() << '\t' << h[0].imag();
    strm << '\t' << h[1].real() << '\t' << h[1].imag();
    strm << '\t' << h[2].real() << '\t' << h[2].imag();
    strm << '\t' << h[3].real() << '\t' << h[3].imag();
    strm << '\t' << n.real() << '\t' << n.imag();
    strm << '\n';
}

int main()
{
    const double J       = 1.;
    const double U       = 1.;
    const double dt      = 0.01;
    const uint64_t steps = 2000;

    blaze::DynamicMatrix<complex<double>> M{{U / 2., -J, -J, U / 2.},
                                            {-J, U / 2., 0., 0.},
                                            {-J, 0., U / 2., 0.},
                                            {U / 2., 0, 0, U / 2.}};
    M *= complex<double>(0., 1.);

    blaze::DynamicVector<complex<double>> h(4);
    h[0] = 1.;

    ieompp::ode::RK4<double> rk4(4, dt);

    ofstream file("trivial.txt");
    for(uint64_t i = 0; i < steps; ++i) {
        write_state(file, i * dt, h);
        rk4.step(M, h);
    }
    write_state(file, steps * dt, h);
    file.close();

    return 0;
}
