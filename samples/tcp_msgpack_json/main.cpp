#include <iostream>
#include <tipe/tipe.hpp>
#include <tipe/nodes/tcp.hpp>
#include <tipe/nodes/msgpack_to_json.hpp>

int main()
{
    auto loop = uvw::Loop::getDefault();

    auto nodes = tip::make_nodes(
        tip::nodes::tcp_server(loop, "localhost", 4242),
        tip::nodes::msgpack_to_json{},
        [](const nlohmann::json& json) {
            std::string v = json["hi"];
            return v;
        },
        tip::nodes::print { std::cout, "\n" }
    );
    auto ids = tip::get_ids(nodes);
    auto& [tcp, m2j, extract, print] = ids;

    auto edges = tip::make_edges(ids)
        .connect(tcp | m2j | extract | print);

    auto g = tip::make_graph(std::move(nodes), edges);

    tip::start_roots(g);
    loop->run();
}
