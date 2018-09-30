//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <stddef.h>
#include <tuple>

namespace tip
{
    template <class... Ts> struct list {};

    template <class... Ts>
    constexpr size_t size(list<Ts...>)
    {
        return sizeof...(Ts);
    }

    template <class... Ts, class T>
    constexpr list<Ts..., T> append(list<Ts...>, T)
    {
        return {};
    }

    template <bool B, class NewT, class Rest>
    struct append_if_match
    {
        using type = Rest;
    };

    template <class NewT, class... Rest>
    struct append_if_match<true, NewT, list<Rest...>>
    {
        using type = list<Rest..., NewT>;
    };

    template <bool B, class NewT, class Rest>
    using append_if_t = typename append_if_match<B, NewT, Rest>::type;

    template<int N, typename... Ts> using type_at =
        typename std::tuple_element<N, std::tuple<Ts...>>::type;

    template<typename... Ts>
    struct select_last
    {
        template<typename U>
        struct tag
        {
            using type = U;
        };

        using type = typename decltype((tag<Ts>{}, ...))::type;
    };

    template <class FunT, class... Elems>
    void map_all(list<Elems...>, FunT&& fun)
    {
        (std::forward<FunT>(fun)(Elems{}), ...);
    }
}