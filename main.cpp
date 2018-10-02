#include <iostream>
#include <vector>
#include <numeric>
#include <tipe/tipe.hpp>
#include <type_traits>
#include <mutex>
#include <future>

struct iota
{
    std::vector<int>
    operator()(int x) const
    {
        std::vector<int> ret(x);
        std::iota(ret.begin(), ret.end(), 1);
        return ret;
    }
};

template <class T>
struct aggregate
{
    explicit aggregate(size_t sz) : m_needed{sz} {}
    aggregate(aggregate&& a) noexcept : m_elems{std::move(a.m_elems)}, m_needed{a.m_needed}, m_prot{} {}

    template <class NextT>
    void operator()(NextT&& next, T&& t) const
    {
        std::lock_guard lk{m_prot};

        m_elems.push_back(std::forward<T>(t));

        if (m_elems.size() < m_needed)
        {
            return;
        }

        next(std::move(m_elems));
        m_elems.clear();
    }

private:
    /**
     * due to const requirement, we have these mutables
     */
    mutable std::vector<T> m_elems;
    mutable std::mutex m_prot;
    size_t m_needed;
};

template <class GraphT>
std::ostream& put(std::ostream& out, const GraphT& graph)
{
    out << graph.node_count() << " nodes\n";
    tip::map_all(graph.get_all_node_ids(), [&](auto n){
        tip::map_all(graph.successors_of(n), [&](auto to){
            out << n.Id << " - " << to.Id << '\n';
        });
    });
    return out;
}

static auto get_graph()
{
    auto nodes = tip::make_nodes(
        [](int x) { return x * 2; },
        aggregate<int> {1000},
        tip::nodes::split{},
        tip::nodes::print{ std::cout, "\n" }
    );

    auto ids = tip::get_ids(nodes);
    auto& [mul, aggr, split, print] = ids;

    auto edges = make_edges(ids)
        .connect(mul | aggr | split | print);

    return make_graph(std::move(nodes), edges);
}

int main()
{
    auto foo = get_graph();
    put(std::cout, foo) << '\n';
    for (int i = 0; i < 1500; ++i) {
        std::async([&foo, i] {
            tip::execute(foo, tip::node_id_t<1>{}, i);
        });
    }
}