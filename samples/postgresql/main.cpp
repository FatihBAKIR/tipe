#include <iostream>
#include <tipe/tipe.hpp>
#include <tipe/nodes/timer.hpp>
#include <tao/postgres.hpp>

int main()
{
    const auto conn = tao::postgres::connection::create("postgresql://postgres:23111994a@home.fatihbakir.net/tutorial");
    conn->execute("INSERT INTO sensor_data(time, temperature) VALUES (NOW(), 33.5)");
    return 0;

    auto loop = uvw::Loop::getDefault();

    using namespace std::chrono_literals;
    auto nodes = tip::make_nodes(
        tip::nodes::timer(loop, 1000ms),
        []{
            return "hello";
        },
        tip::nodes::print { std::cout, "\n" }
    );
    auto ids = tip::get_ids(nodes);
    auto& [tmr, hello, print] = ids;

    auto edges = tip::make_edges(ids)
        .connect(tmr | hello | print);

    auto g = tip::make_graph(std::move(nodes), edges);

    tip::start_roots(g);
    loop->run();
}
