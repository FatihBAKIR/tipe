//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <tipe/meta/list.hpp>
#include <tuple>
#include "node.hpp"

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
}
