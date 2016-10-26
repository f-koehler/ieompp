#ifndef CONSTRAINTS_HPP_
#define CONSTRAINTS_HPP_

#include <algorithm>
#include <ostream>
#include <tuple>
#include <vector>

namespace ieompp
{
    template <typename MomentumIndexT, typename PrefactorT>
    struct MomentumConservation {
        using MomentumIndex = MomentumIndexT;
        using Prefactor     = PrefactorT;

        std::vector<std::tuple<Prefactor, MomentumIndex>> momenta;

        void add_momentum(const MomentumIndex& index, const Prefactor& prefactor = Prefactor(1.))
        {
            momenta.push_back(std::make_tuple(prefactor, index));
        }
    };


    template <typename MomentumIndex, typename Prefactor>
    std::ostream& operator<<(std::ostream& strm,
                             const MomentumConservation<MomentumIndex, Prefactor>& con)
    {
        strm << u8"δ_{";
        std::for_each(con.momenta.begin(), con.momenta.end() - 1,
                      [&strm](const std::tuple<Prefactor, MomentumIndex>& x) {
                          strm << std::showpos << std::get<0>(x) << u8"⋅" << std::noshowpos
                               << std::get<1>(x);
                      });
        strm << ", " << std::showpos << -std::get<0>(con.momenta.back()) << u8"⋅" << std::noshowpos
             << std::get<1>(con.momenta.back()) << "}";
        return strm;
    }
}

#endif
