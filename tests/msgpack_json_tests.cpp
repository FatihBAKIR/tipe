//
// Created by fatih on 10/1/18.
//

#include "catch.hpp"
#include <cwpack.hpp>
#include <tipe/nodes/msgpack_to_json.hpp>
#include <tipe/tipe.hpp>

template <class T>
void one_elem_test(T t)
{
    char buf[512];
    tos::msgpack::packer p{buf};
    p.insert(t);

    tip::nodes::msgpack_to_json to_json;
    auto res = to_json(p.get());

    REQUIRE(res == t);
}

TEST_CASE("basic tests", "[msgpack-to-json]")
{
    one_elem_test(true);
    one_elem_test(false);
    one_elem_test(nullptr);
    one_elem_test(1);
    one_elem_test(-1);
    one_elem_test(0);
    one_elem_test(1.0f);
    one_elem_test(-1.0f);
    one_elem_test(0.0f);
    one_elem_test(-0.0f);
    one_elem_test(1.0);
    one_elem_test(-1.0);
    one_elem_test(0.0);
    one_elem_test(-0.0);
    one_elem_test(std::string("hello world"));
}

TEST_CASE("array test", "[msgpack-to-json, array]")
{
    char buf[512];
    tos::msgpack::packer p{buf};
    auto arr = p.insert_arr(2);
    arr.insert(5);
    auto arr2 = arr.insert_arr(2);
    arr2.insert(6);
    arr2.insert(7);

    tip::nodes::msgpack_to_json to_json;
    auto res = to_json(p.get());

    REQUIRE(res[0] == 5);
    REQUIRE(res[1][0] == 6);
    REQUIRE(res[1][1] == 7);
}


TEST_CASE("map test", "[msgpack-to-json, map]")
{
    char buf[512];
    tos::msgpack::packer p{buf};
    auto arr = p.insert_map(2);
    arr.insert("hi", 5);
    auto arr2 = arr.insert_map("yo", 2);
    arr2.insert("damn", 6);
    arr2.insert("whadup", 7);

    tip::nodes::msgpack_to_json to_json;
    auto res = to_json(p.get());

    REQUIRE(res["hi"] == 5);
    REQUIRE(res["yo"]["damn"] == 6);
    REQUIRE(res["yo"]["whadup"] == 7);
}

TEST_CASE("msgpack to json as node", "[msgpack-to-json, as-node]")
{
    bool execd = false;
    auto nodes = tip::make_nodes(
        tip::nodes::msgpack_to_json{},
        [&](const nlohmann::json& json){
            REQUIRE(json["k"] == "v");
            execd = true;
        }
    );

    auto [m2j, out] = tip::get_ids(nodes);

    auto edges = tip::make_edges(tip::get_ids(nodes))
            .connect(m2j | out);

    auto g = tip::make_graph(std::move(nodes), edges);

    char buf[512];
    tos::msgpack::packer p{buf};
    auto arr = p.insert_map(1);
    arr.insert("k", "v");

    tip::execute(g, m2j, p.get());
    REQUIRE(execd);
}