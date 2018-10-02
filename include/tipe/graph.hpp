//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <tipe/meta/list.hpp>
#include <tuple>
#include <tipe/node.hpp>
#include <tipe/edge.hpp>

namespace tip
{
    template <
        class Storage,
        class Edges,
        class... NodeTs
    >
    struct graph
    {
        using NodesT = list<NodeTs...>;
        using EdgesList = Edges;

        constexpr size_t node_count() const
        {
            return sizeof...(NodeTs);
        }

        constexpr auto get_all_node_ids() const
        {
            return get_id_list(list<NodeTs...>{});
        }

        template <size_t NodeId>
        auto& get_node(node_id_t<NodeId> node)
        {
            return std::get<NodeId - 1>(m_nodes);
        }

        template <size_t NodeId>
        constexpr auto successors_of(node_id_t<NodeId> node) const
        {
            return Edges::successors(node);
        }

        template <size_t NodeId>
        using successor_ts = typename Edges:: template successor_ts<NodeId>;

        Storage m_nodes;
    };

    template <class... NodeTs, class... Edges>
    constexpr auto make_graph(std::tuple<NodeTs...>&& nodes, tip::edges<Edges...>)
    {
        return tip::graph<std::tuple<NodeTs...>, tip::edges<Edges...>, NodeTs...> {
                std::move(nodes)
        };
    }

    template <class GraphT, size_t N1, size_t N2>
    constexpr auto edge_decltype(GraphT& graph, edge<N1, N2>);
}
