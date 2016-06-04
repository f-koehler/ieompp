#ifndef TUPLE_HPP_
#define TUPLE_HPP_

#include <ostream>
#include <string>
#include <tuple>

namespace ieompp
{
    namespace detail
    {
        template <typename Tuple, std::size_t I, std::size_t N>
        struct TuplePrinter
        {
            static void print(std::ostream& strm, const Tuple& t, const std::string& delimiter) {
                strm << std::get<N>(t) << delimiter;
                TuplePrinter<Tuple, I + 1, N>::print(strm, t, delimiter);
            }
        };

        template <typename Tuple, std::size_t N>
        struct TuplePrinter<Tuple, N, N>
        {
            static void print(std::ostream& strm, const Tuple& t, const std::string& delimiter) {
                (void)delimiter;
                strm << std::get<N>(t);
            }
        };
    }

    template <typename... Ts>
    void print_tuple(std::ostream& strm, const std::tuple<Ts...>& t,
                     const std::string& delimiter = ",", const std::string& left = "(",
                     const std::string& right = ")")
    {
        strm << left;
        detail::TuplePrinter<std::tuple<Ts...>, 0, sizeof...(Ts)-1>::print(strm, t, delimiter);
        strm << right;
    }
}

#endif
