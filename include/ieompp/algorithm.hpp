#ifndef IEOMPP_ALGORITHM_HPP_
#define IEOMPP_ALGORITHM_HPP_

#include <algorithm>
#include <list>

namespace ieompp
{
    namespace on_merge
    {
        template <typename T>
        struct Ignore {
            void operator()(T&, const T&) const {}
        };

        template <typename T>
        struct Add {
            void operator()(T& a, const T& b) const { a += b; }
        };
    }


    template <typename InputIt, typename OutputIt, typename MergeFunction, typename EqualityTest>
    void globally_unique_with_merge(InputIt first, InputIt last, OutputIt result, MergeFunction f, EqualityTest eq)
    {
        std::list<typename std::remove_const<typename InputIt::value_type>::type> lst;
        for(auto it = first; it != last; ++it) {
            auto pos = std::lower_bound(lst.begin(), lst.end(), *it);
            if(pos == lst.end()) {
                lst.push_back(*it);
                continue;
            }
            if(eq(*pos, *it)) f(*pos, *it);
            else lst.insert(pos, *it);
        }
        std::copy(lst.begin(), lst.end(), result);
    }
}

#endif
