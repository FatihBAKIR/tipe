#include <iostream>
#include <tipe/tipe.hpp>
#include <tipe/nodes/tcp.hpp>
#include <tipe/nodes/msgpack_to_json.hpp>
#include <tao/postgres/connection.hpp>

int main()
{
    auto loop = uvw::Loop::getDefault();

    auto nodes = tip::make_nodes(
        tip::nodes::tcp_server(loop, "localhost", 4242),
        tip::nodes::msgpack_to_json{},
        [](const nlohmann::json& json) {
            std::vector<float> v = json;
            return v;
        },
        [](const std::vector<float>& data) -> void
        {
            const auto conn = tao::postgres::connection::create("postgresql://postgres:23111994a@home.fatihbakir.net/tutorial");
            conn->execute("INSERT INTO samples(time, temperature, humidity) VALUES (NOW(), $1, $2)", data[0], data[1]);
        },
        tip::nodes::split{},
        tip::nodes::print { std::cout, "\n" }
    );
    auto ids = tip::get_ids(nodes);
    auto& [tcp, m2j, extract, database, split, print] = ids;

    auto edges = tip::make_edges(ids)
        .connect(tcp | m2j | extract)
        .connect(extract | split | print)
        .connect(extract | database);

    auto g = tip::make_graph(std::move(nodes), edges);

    tip::start_roots(g);
    loop->run();
}
