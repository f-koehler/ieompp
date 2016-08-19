#ifndef IEOMPP_ALGORITHM_HPP_
#define IEOMPP_ALGORITHM_HPP_

#include <algorithm>
#include <functional>
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

    namespace detail
    {
        template <typename InputIt>
        struct GUWMHelper {
            using Value           = typename std::remove_const<typename InputIt::value_type>::type;
            using BinaryPredicate = std::function<bool(const Value&, const Value&)>;
            using MergeFunction   = std::function<void(Value&, const Value&)>;
        };
    }

    template <typename InputIt, typename OutputIt>
    void
    globally_unique_with_merge(InputIt first, InputIt last, OutputIt out,
                               typename detail::GUWMHelper<InputIt>::MergeFunction f =
                                   on_merge::Ignore<typename detail::GUWMHelper<InputIt>::Value>(),
                               typename detail::GUWMHelper<InputIt>::BinaryPredicate eq =
                                   std::equal_to<typename detail::GUWMHelper<InputIt>::Value>(),
                               typename detail::GUWMHelper<InputIt>::BinaryPredicate order =
                                   std::less<typename detail::GUWMHelper<InputIt>::Value>())
    {
        std::list<typename std::remove_const<typename InputIt::value_type>::type> lst;
        for(auto it = first; it != last; ++it) {
            auto pos = std::lower_bound(lst.begin(), lst.end(), *it, order);
            if(pos == lst.end()) {
                lst.push_back(*it);
                continue;
            }
            if(eq(*pos, *it)) f(*pos, *it);
            else lst.insert(pos, *it);
        }
        std::copy(lst.begin(), lst.end(), out);
    }
}

#endif
