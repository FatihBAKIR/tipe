#include <iostream>
#include <tipe/tipe.hpp>
#include <tipe/nodes/timer.hpp>

int main()
{
    auto loop = uvw::Loop::getDefault();
    using namespace std::chrono_literals;
    auto nodes = tip::make_nodes(
        tip::nodes::timer(loop, 1000ms),
        []() -> std::string {
            return "hello";
        },
        tip::nodes::print { std::cout, "\n" }
    );
    auto ids = tip::get_ids(nodes);
    auto& [tmr, hello, print] = ids;

    auto edges = tip::make_edges(ids)
        .connect(tmr | hello | print);

    auto g = tip::make_graph(std::move(nodes), edges);

    auto r = g.roots();

    tip::map_all(r, [&g](auto root_id){
        auto& ts = g.get_node(root_id);
        ts.tip_start(g, next_of(g, root_id));
    });

    loop->run();
}
