
//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <utility>
#include <tipe/node.hpp>
#include "flow_ctx.hpp"

namespace tip
{
    namespace detail
    {
        /**
         * Just a gadget to determine if a function object is callable with the
         * given list of types
         */
        template<class F, class...Args>
        struct is_callable
        {
            template<class U> static auto test(U* p) -> decltype(std::declval<U>()(std::declval<Args>()...), void(), std::true_type());
            template<class U> static auto test(...) -> decltype(std::false_type());

            static constexpr bool value = decltype(test<F>(nullptr))::value;
        };
    }

    template <class GraphT, size_t NodeId>
    constexpr auto start_of(GraphT& graph, tip::node_id_t<NodeId> n)
    {
        return [&graph, n] (auto ctx, auto&&... val)
        {
            using successors = typename GraphT :: template successor_ts<n.Id>;
            map_all(successors{}, [&](auto el){
                using next_t = decltype(el);
                execute(graph, next_t{}, ctx, std::forward<decltype(val)>(val)...);
            });
        };
    }

    template <class GraphT, size_t NodeId, class CtxT = empty_ctx>
    constexpr auto next_of(GraphT& graph, tip::node_id_t<NodeId> n, CtxT& ctx)
    {
        return [&graph, n, &ctx] (auto&&... val)
        {
            using successors = typename GraphT :: template successor_ts<n.Id>;
            map_all(successors{}, [&](auto el){
                using next_t = decltype(el);
                execute(graph, next_t{}, ctx, std::forward<decltype(val)>(val)...);
            });
        };
    }

    template <size_t RootId, class GraphT, class CtxT = empty_ctx, class... ArgTs>
    constexpr auto execute(GraphT& graph, tip::node_id_t<RootId> n, CtxT& ctx, ArgTs&&... args)
    {
        /**
         * This lambda calls all the successors of the current node with the
         * value passed by the current node. It's either called directly by
         * the node, or we wrap the node and call it with the return value.
         */
        auto next_fun = next_of(graph, n, ctx);

        // Node identifiers are 1 indexed
        const auto& node = std::get<RootId - 1>(graph.m_nodes);

        /**
         * If the node uses the high level interface, we wrap it so that we can
         * call the next function.
         */
        auto wrapper = [&](auto&& next, auto&&... arguments)
        {
            if constexpr (detail::is_callable<std::remove_reference_t<decltype(node)>, decltype((ctx)), ArgTs...>::value)
            {
                using ret_t = decltype(node(std::forward<decltype(ctx)>(ctx), std::forward<decltype(arguments)>(arguments)...));
                if constexpr (std::is_same_v<ret_t, void>)
                {
                    node(std::forward<decltype(ctx)>(ctx), std::forward<decltype(arguments)>(arguments)...);
                    next();
                }
                else
                {
                    next(std::forward<decltype(ctx)>(ctx), node(std::forward<decltype(arguments)>(arguments)...));
                }
            }
            else
            {
                using ret_t = decltype(node(std::forward<decltype(arguments)>(arguments)...));

                if constexpr (std::is_same_v<ret_t, void>)
                {
                    node(std::forward<decltype(arguments)>(arguments)...);
                    next();
                }
                else
                {
                    next(node(std::forward<decltype(arguments)>(arguments)...));
                }
            }
        };

        /**
         * If the function is callable with the given arguments, it means it's not expecting
         * a next parameter, thus, it implements the high level API.
         */
        if constexpr (detail::is_callable<std::remove_reference_t<decltype(node)>, decltype((ctx)), decltype((next_fun)), ArgTs...>::value)
        {
            node(ctx, next_fun, std::forward<ArgTs>(args)...);
        }
        else if constexpr (detail::is_callable<std::remove_reference_t<decltype(node)>, decltype((next_fun)), ArgTs...>::value)
        {
            node(next_fun, std::forward<ArgTs>(args)...);
        }
        else
        {
            wrapper(next_fun, std::forward<ArgTs>(args)...);
        }
    }
}
