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

        static constexpr size_t node_count()
        {
            return sizeof...(NodeTs);
        }

        template <size_t NodeId>
        static constexpr auto successors_of(node_id_t<NodeId> node)
        {
            return Edges::successors(node);
        }

        Storage m_nodes;
    };

    template <class Edges, size_t Len>
    struct graph_def
    {
    };

    template <class... NodeTs, class... Edges>
    constexpr auto make_graph(std::tuple<NodeTs...>&& nodes, tip::edges<Edges...>)
    {
        return tip::graph<std::tuple<NodeTs...>, tip::edges<Edges...>, NodeTs...> {
                std::move(nodes)
        };
    }

    template <class GraphT, size_t N1, size_t N2>
    constexpr auto edge_decltype(GraphT& graph, edge<N1, N2>)
    {

    }
}
