//
// Created by Mehmet Fatih BAKIR on 29/09/2018.
//

#include <tipe/tipe.hpp>
#include "catch.hpp"

TEST_CASE("very basic", "[tipe]")
{
    int res;

    auto nodes = tip::make_nodes(
        []{ return 1; },
        [](auto x) { return x + x; },
        [](auto x) { return x * x; },
        [&res](auto x) { res = x; }
    );

    auto ids = tip::get_ids(nodes);
    auto [base, add, mult, fin] = ids;

    auto edges = tip::make_edges(ids)
        .connect(base | add | mult | fin);

    auto g = tip::make_graph(std::move(nodes), edges);

    tip::execute(g, base);

    REQUIRE(res == 4);
}