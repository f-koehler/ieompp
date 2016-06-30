#ifndef IEOMPP_FERMI_SEA_HPP_
#define IEOMPP_FERMI_SEA_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <tuple>

#include <ieompp/types/complex.hpp>

namespace ieompp
{
    template <typename Term>
    struct FermiSea {
        using Real    = typename types::real_type<typename Term::Prefactor>::type;
        using Indices = typename Term::Operator::Indices;

        template <typename FourierSpace, typename Dispersion>
        bool expectation_value_vanishes(const Term& term, const FourierSpace& fourier_space,
                                        const Dispersion& dispersion,
                                        const Real& fermi_energy = Real(0.)) const
        {
            std::list<std::tuple<bool, Indices>> effect;

            for(auto& op : term.operators) {
                const auto indices = op.index_tuple();
                auto pos           = std::find_if(effect.begin(), effect.end(),
                                        [&indices](const std::tuple<bool, Indices>& entry) {
                                            return std::get<1>(entry) == indices;
                                        });
                if(pos == effect.end()) {
                    // TODO: currently the first index will be used
                    // TODO: only store relevant index
                    const auto momentum = fourier_space[std::get<0>(indices)];
                    const auto energy   = dispersion(momentum);
                    if(energy < fermi_energy) {
                        if(op.creator) return true;
                        else effect.push_back(std::make_tuple(false, indices));
                    } else {
                        if(!op.creator) return true;
                        else effect.push_back(std::make_tuple(true, indices));
                    }
                    continue;
                }

                if(std::get<0>(*pos) == op.creator) return true;
                effect.erase(pos);
            }

            return !effect.empty();
        }
    };
}

#endif
