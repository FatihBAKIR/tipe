//
// Created by Mehmet Fatih BAKIR on 02/10/2018.
//

#include <tipe/tipe.hpp>
#include "catch.hpp"
#include <thread>
#include <atomic>

TEST_CASE("simple parallelism", "[tipe]")
{
    std::atomic<int> res;

    auto nodes = tip::make_nodes(
        []{ return 1; },
        [](auto x) { return x * x; },
        [&res](auto x) { ++res; }
    );

    auto ids = tip::get_ids(nodes);
    auto [base, sq, fin] = ids;

    auto edges = tip::make_edges(ids)
            .connect(base | sq | fin);

    auto g = tip::make_graph(std::move(nodes), edges);

    std::vector<std::thread> ts;
    ts.reserve(10);
    for (int i = 0; i < 10; ++i)
    {
        ts.emplace_back([&]{
            auto ids = tip::get_ids(nodes);
            auto [base, sq, fin] = ids;
            for (auto j = 0; j < 1'000; ++j)
            {
                tip::execute(g, base);
            }
        });
    }

    for (auto& t : ts)
    {
        t.join();
    }

    REQUIRE(res == 10'000);
}
