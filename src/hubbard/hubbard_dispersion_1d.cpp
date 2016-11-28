#include <ieompp/lattices/periodic_chain.hpp>
#include <ieompp/models/hubbard_common/dispersion.hpp>
using namespace ieompp;

#include <fstream>
using namespace std;

int main()
{
    const uint64_t N = 1024;
    const lattices::PeriodicChain<double, uint64_t> brillouin_zone(N), lattice(N, 1.);
    const auto dispersion = models::hubbard_common::make_dispersion(brillouin_zone, lattice, 1.);

    ofstream file("dispersion.txt");
    for(const auto& site : brillouin_zone) {
        file << brillouin_zone[site] << '\t' << dispersion(site) << '\n';
    }
    file.close();
}
