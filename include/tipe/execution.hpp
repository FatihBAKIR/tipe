
//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#pragma once

#include <utility>
#include <tipe/node.hpp>

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

    template <size_t RootId, class GraphT, class... ArgTs>
    constexpr auto execute(GraphT& graph, tip::node_id_t<RootId> n, ArgTs&&... args)
    {
        /**
         * This lambda calls all the successors of the current node with the
         * value passed by the current node. It's either called directly by
         * the node, or we wrap the node and call it with the return value.
         */
        auto next_fun = [&](auto&&... val)
        {
            constexpr auto successors = GraphT::successors_of(n);
            map_all(successors, [&](auto el){
                using next_t = decltype(el);
                execute(graph, next_t{}, std::forward<decltype(val)>(val)...);
            });
        };

        // Node identifiers are 1 indexed
        const auto& node = std::get<RootId - 1>(graph.m_nodes);

        /**
         * If the node uses the high level interface, we wrap it so that we can
         * call the next function.
         */
        auto wrapper = [&](auto&& next, auto&&... arguments)
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
        };

        /**
         * If the function is callable with the given arguments, it means it's not expecting
         * a next parameter, thus, it implements the high level API.
         */
        if constexpr (detail::is_callable<std::remove_reference_t<decltype(node)>, ArgTs...>::value)
        {
            wrapper(next_fun, std::forward<ArgTs>(args)...);
        }
        else
        {
            node(next_fun, std::forward<ArgTs>(args)...);
        }
    }
}
