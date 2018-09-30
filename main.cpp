#include <iostream>
#include <vector>
#include <numeric>
#include <tipe/tipe.hpp>

struct node
{
    std::vector<int>
    operator()(int x) const
    {
        std::vector<int> ret(x);
        std::iota(ret.begin(), ret.end(), x);
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

template <size_t RootId, class GraphT, class... ArgTs>
auto execute(GraphT& graph, tip::node_id_t<RootId>, ArgTs&&... args)
{
    const auto& node = std::get<RootId - 1>(graph.m_nodes);
    auto res = node(std::forward<ArgTs>(args)...);
    return res;
}

auto get_graph()
{
    auto nodes = tip::make_nodes(
        tip::nodes::constant<1>{},
        node{},
        tip::nodes::split{},
        node{},
        tip::nodes::split{},
        print{ std::cout, ",\n" }
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
    auto res = execute(foo, tip::node_id_t<2>{}, 3);
    return 0;
}