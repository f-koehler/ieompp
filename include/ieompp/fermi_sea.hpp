#ifndef IEOMPP_FERMI_SEA_HPP_
#define IEOMPP_FERMI_SEA_HPP_

#include <algorithm>
#include <tuple>
#include <vector>

namespace ieompp
{
    template <typename Term>
    bool expectation_value_vanishes(const Term& t)
    {
        using Indices = typename Term::Operator::Indices;
        std::vector<std::tuple<bool, Indices>> effect;

        for(auto& op : t.operators) {
            auto indices = op.index_tuple();
            auto pos     = std::find_if(effect.begin(), effect.end(),
                                    [&indices](const std::tuple<bool, Indices>& entry) {
                                        return std::get<1>(entry) == indices;
                                    });
        }
    }
}

#endif
