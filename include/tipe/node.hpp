//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <tipe/meta/list.hpp>
#include <tuple>

namespace tip
{

    template <size_t NodeId>
    struct node_id_t
    {
        static constexpr auto Id = NodeId;
    };

    template <size_t... Ids>
    constexpr std::tuple<node_id_t<Ids + 1>...>
    make_node_ids(std::index_sequence<Ids...>) noexcept
    {
        return {};
    }

    template <class... NodeTs>
    constexpr auto get_ids(const std::tuple<NodeTs...>& nodes) noexcept
    {
        return make_node_ids(std::make_index_sequence<sizeof...(NodeTs)>{});
    }

    template <class... NodeTs>
    constexpr std::tuple<NodeTs...> make_nodes(NodeTs&&... nodes)
    {
        return { std::forward<NodeTs>(nodes)... };
    }
}
