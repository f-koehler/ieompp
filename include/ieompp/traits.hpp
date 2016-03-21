#ifndef IEOMPP_TRAITS_HPP_
#define IEOMPP_TRAITS_HPP_

#include <type_traits>

namespace ieompp
{
    template <typename T, typename... Types>
    struct is_one_of;

    template <typename T, typename OtherT, typename... Types>
    struct is_one_of<T, OtherT, Types...> : public is_one_of<T, Types...> {
        static constexpr auto value =
            std::is_same<T, OtherT>::value || is_one_of<T, Types...>::value;
    };

    template <typename T, typename OtherT>
    struct is_one_of<T, OtherT> {
        static constexpr auto value = std::is_same<T, OtherT>::value;
    };
}


#endif
