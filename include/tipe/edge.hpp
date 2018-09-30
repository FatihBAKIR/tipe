//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <tipe/node.hpp>

namespace tip
{
    /**
     * Creates an empty edge list for the given nodes.
     * @tparam NodeIds node ids as a tuple.
     * @return an empty edge list
     */
    template<size_t... NodeIds>
    constexpr auto make_edges(const std::tuple<node_id_t<NodeIds>...>&);

    /**
     * This class template represents an edge in a `tipe` graph.
     * As the edges in `tipe` graphs are directed, there exists a
     * distinction between the source node and the destionation node.
     */
    template <size_t Tag1, size_t Tag2>
    struct edge {
        static constexpr size_t source = Tag1;
        static constexpr size_t dest = Tag2;
    };

    namespace detail
    {
        template <size_t>
        using empty_list = tip::list<>;
    }

    template <class... Edges>
    struct edges
    {
        /**
         * Create connections from the given list of edges.
         *
         * This list should be generated through the use of pipe operator
         * between node identifiers.
         */
        template <class First, class... NewEdges>
        constexpr auto connect(list<First, NewEdges...> edges) &&
        {
            if constexpr(sizeof...(NewEdges) > 0)
            {
                return do_connect(First{}).connect(list<NewEdges...>{});
            }
            else
            {
                return do_connect(First{});
            }
        }

        /**
         * Returns the node ids of the successors of a given node.
         * @param NodeId id of the node to get the successors of
         * @return id's successors of the given node in a list
         */
        template <size_t NodeId>
        static constexpr auto successors(node_id_t<NodeId> node)
        {
            return type_at<NodeId - 1, Edges...>{};
        }

    private:

        template <size_t N1, size_t N2>
        static constexpr auto do_connect(edge<N1, N2> e)
        {
            return do_connect(e, std::make_index_sequence<sizeof...(Edges)>{});
        };

        template <size_t N1, size_t N2, size_t... Ids>
        static constexpr auto do_connect(edge<N1, N2>, std::index_sequence<Ids...>)
        {
            return edges<append_if_t<Ids == N1 - 1, node_id_t<N2>, Edges>...>{};
        };
    };

    template<size_t... NodeIds>
    constexpr auto make_edges(const std::tuple<node_id_t<NodeIds>...>&) {
        return edges<detail::empty_list<NodeIds>...>{};
    }

    template <size_t N1, size_t N2>
    constexpr list<edge<N1, N2>> operator|(node_id_t<N1>, node_id_t<N2>)
    {
        return {};
    }

    template <class... Edges, size_t New>
    constexpr auto operator |(list<Edges...>, node_id_t<New>)
    {
        using LastT = typename select_last<Edges...>::type;
        return list<Edges..., edge<LastT::dest, New>>{};
    }
}
