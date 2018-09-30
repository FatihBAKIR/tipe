#include <iostream>
#include <vector>
#include <numeric>
#include <tipe/tipe.hpp>
#include <type_traits>

struct node
{
    std::vector<int>
    operator()(int x) const
    {
        std::vector<int> ret(x);
        std::iota(ret.begin(), ret.end(), 1);
        return ret;
    }
};

struct print
{
public:
    constexpr print(std::ostream& os, std::string_view sep)
        : m_osp{&os}, m_sep{sep} {
    }

    template <class T>
    void operator()(T&& t) const
    {
        *m_osp << t << m_sep;
    }

private:
    mutable std::ostream* m_osp;
    std::string_view m_sep;
};

template <class... NodeTs, class... Edges>
constexpr auto make_graph(std::tuple<NodeTs...>&& nodes, tip::edges<Edges...>)
{
    return tip::graph<std::tuple<NodeTs...>, tip::edges<Edges...>, NodeTs...> {
        std::move(nodes)
    };
}

template<class F, class...Args>
struct is_callable
{
    template<class U> static auto test(U* p) -> decltype(std::declval<U>()(std::declval<Args>()...), void(), std::true_type());
    template<class U> static auto test(...) -> decltype(std::false_type());

    static constexpr bool value = decltype(test<F>(nullptr))::value;
};

template <size_t RootId, class GraphT, class... ArgTs>
constexpr auto execute(GraphT& graph, tip::node_id_t<RootId> n, ArgTs&&... args)
{
    auto next_fun = [&](auto&&... val)
    {
        constexpr auto successors = GraphT::successors_of(n);
        map_all(successors, [&](auto el){
            using next_t = decltype(el);
            execute(graph, next_t{}, std::forward<decltype(val)>(val)...);
        });
    };

    const auto& node = std::get<RootId - 1>(graph.m_nodes);

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

    if constexpr (is_callable<std::remove_reference_t<decltype(node)>, ArgTs...>::value)
    {
        wrapper(next_fun, std::forward<ArgTs>(args)...);
    }
    else
    {
        node(next_fun, std::forward<ArgTs>(args)...);
    }
}

auto get_graph()
{
    auto nodes = tip::make_nodes(
        tip::nodes::constant<4>{},
        node{},
        tip::nodes::split{},
        node{},
        tip::nodes::split{},
        print{ std::cout, "\n" }
    );

    auto ids = tip::get_ids(nodes);
    auto& [entry, iota1, split1, iota2, split2, print] = ids;

    auto edges = make_edges(ids)
        .connect(entry | iota1 | split1 | iota2 | split2 | print);

    return make_graph(std::move(nodes), edges);
}

int main()
{
    auto foo = get_graph();
    execute(foo, tip::node_id_t<1>{});
    return 0;
}