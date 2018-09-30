//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <tipe/meta/list.hpp>
#include <tuple>

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

        Storage m_nodes;
    };

    template <class Edges, size_t Len>
    struct graph_def
    {
    };
}
