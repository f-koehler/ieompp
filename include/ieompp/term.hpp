#ifndef TERM_HPP_
#define TERM_HPP_

#include <vector>

namespace ieompp
{
    template <typename OperatorT, typename ContainerT = std::vector<OperatorT>>
    struct Term {
        using Operator  = OperatorT;
        using Container = ContainerT;

        Container operators;
    };
}

#endif
